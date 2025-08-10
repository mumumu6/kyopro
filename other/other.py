import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import math, torch, torch.nn as nn
import torch.nn.functional as F

class MyEmbedding(nn.Module):
    def __init__(self,
                 vocab_size: int,
                 embed_dim:  int,
                 padding_idx: int = 0, # 学習する際文章ごとに長さは異なるため <pad>のようにパディングトークンを作成する。idが0だったらpaddingだよという意味
                 std: float = 0.02 # 重みを正規化するための標準偏差
                 ):
        super().__init__()

        self.weight = nn.Parameter(torch.empty(vocab_size, embed_dim)) # 単語の意味を表現するベクトル。語彙数分だけ次元がある。
        # 正規分布で初期化
        nn.init.normal_(self.weight, mean=0.0, std=std)
        self.padding_idx = padding_idx

    def forward(self, input_ids: torch.Tensor) -> torch.Tensor:
        """
        input_ids: (B, T)  — int64
        return   : (B, T, embed_dim) — float32/float16
        """

        out = self.weight[input_ids]


        if self.padding_idx is not None:
            mask = (input_ids == self.padding_idx).unsqueeze(-1)
            out = out.masked_fill(mask, 0.0) # paddingは0にして返却する。
        return out

class PositionalEncoding(nn.Module):
    def __init__(self, d_model: int, max_len: int = 512):
        super().__init__()
        pe = torch.zeros(max_len, d_model)
        pos = torch.arange(0, max_len, dtype=torch.float32).unsqueeze(1) # 0 からmax_lenまでの連続した位置を表す配列
        div = torch.exp(torch.arange(0, d_model, 2).float() *
                        (-math.log(10000.0) / d_model)) # 分数部分　こうかくと早いらしい？
        pe[:, 0::2] = torch.sin(pos * div) # 偶数番目はsin
        pe[:, 1::2] = torch.cos(pos * div) # 奇数番目はcos
        self.register_buffer("pe", pe.unsqueeze(0))   # バッチ処理に対応するためunsqueeze

    def forward(self, x):
        return x + self.pe[:, :x.size(1)] # 位置を加える


def scaled_dot_product_attention(Q, K, V,mask=None):
    d_k = Q.size(-1)
    scores = (Q @ K.transpose(-2, -1)) / math.sqrt(d_k)  # (B, h, T, d_model)　バッチサイズ、ヘッド数、系列長、ヘッドの数
    if mask is not None: # embedding層でも0にしているが0ベクトルは無意味なベクトルではなく空間の中で0という点を意味してしまっているのでattentionを-infにする(soft maxで0になる)
        scores = scores.masked_fill(mask == 0, float("-inf"))
    attn = torch.softmax(scores, dim=-1)
    return attn @ V  # (B, h, T, d_k)


class MultiHeadAttention(nn.Module):
    def __init__(self, d_model, n_heads, dropout=0.1):
        super().__init__()
        assert d_model % n_heads == 0 # 単語の意味ベクトルの次元がヘッドの数で割り切れなかったらアサーション
        self.d_k = d_model // n_heads
        self.n_heads = n_heads
        self.w_q = nn.Linear(d_model, d_model) # マルチヘッドアテンションのための行列
        self.w_k = nn.Linear(d_model, d_model)
        self.w_v = nn.Linear(d_model, d_model)
        self.fc  = nn.Linear(d_model, d_model) # 最後に行う線形変換。ヘッドで分割した情報を混ぜる？
        self.dropout = nn.Dropout(dropout) # dropout層

    def forward(self, x, mask=None):
        B, T, _ = x.size()
        # 1) 線形変換 → (B, T, d_model) → reshape (B, h, T, d_k)
        Q = self.w_q(x).view(B, T, self.n_heads, self.d_k).transpose(1,2)
        K = self.w_k(x).view(B, T, self.n_heads, self.d_k).transpose(1,2)
        V = self.w_v(x).view(B, T, self.n_heads, self.d_k).transpose(1,2)
        # 2) Attention
        out = scaled_dot_product_attention(Q, K, V, mask)
        # 3) concat heads → (B, T, d_model)
        out = out.transpose(1,2).contiguous().view(B, T, self.n_heads*self.d_k) # concatする
        return self.fc(self.dropout(out))


class FeedForward(nn.Module):
    def __init__(self, d_model: int, d_ff: int = None, p: float = 0.1):
        super().__init__()

        d_ff = d_ff or 4 * d_model # 中間層の次元

        self.net = nn.Sequential(
            nn.Linear(d_model, d_ff),
            nn.GELU(), # RELUのような見た目だが連続でなめらか
            nn.Dropout(p),
            nn.Linear(d_ff, d_model),
            nn.Dropout(p),
        )
    def forward(self, x):            # (B, T, d_model)
        return self.net(x)


class EncoderBlock(nn.Module):
    def __init__(self, d_model: int, n_heads: int, d_ff: int, p: float = 0.1):
        super().__init__()
        self.ln1  = nn.LayerNorm(d_model)
        self.attn = MultiHeadAttention(d_model, n_heads, dropout=p)
        self.ln2  = nn.LayerNorm(d_model)
        self.ffn  = FeedForward(d_model, d_ff, p)

    def forward(self, x, mask=None):
        # 残差 1：Attention
        x = x + self.attn(self.ln1(x), mask)
        # 残差 2：FFN
        x = x + self.ffn(self.ln2(x))
        return x

class NewsTransformerClassifier(nn.Module):
    def __init__(self,
                 vocab_size: int,
                 num_labels: int = 4,
                 d_model: int = 128,
                 n_heads: int = 4,
                 num_layers: int = 2,
                 max_len: int = 64,
                 d_ff: int = None):
        super().__init__()
        d_ff = d_ff or 4 * d_model

        self.emb  = MyEmbedding(vocab_size, d_model, padding_idx=0)
        self.pos  = PositionalEncoding(d_model, max_len)

        self.layers = nn.ModuleList([
            EncoderBlock(d_model, n_heads, d_ff) for _ in range(num_layers)
        ])
        self.norm = nn.LayerNorm(d_model) #正規化

        self.classifier = nn.Sequential(
            nn.Dropout(0.1),
            nn.Linear(d_model, num_labels)
        ) # 今回のタスクである　４カテゴリに分類する

    def forward(self, input_ids):           # (B, T)
        # パディングマスク (B, 1, 1, T) でAttentionに渡す
        pad_mask = (input_ids != 0).unsqueeze(1).unsqueeze(2)

        x = self.pos(self.emb(input_ids))   # (B, T, d_model)　 inputのid列をembeddingしてposition encoding
        for layer in self.layers:
            x = layer(x, pad_mask)

        cls_vec = self.norm(x)[:, 0]        # 先頭トークン
        return self.classifier(cls_vec)     # (B, 4)


# 訓練データ (120,000件)
train_url = "https://raw.githubusercontent.com/mhjabreel/CharCnn_Keras/master/data/ag_news_csv/train.csv"
train_df = pd.read_csv(train_url, header=None)
train_df.columns = ["label", "title", "description"]

# テストデータ（7,600件）
test_url = "https://raw.githubusercontent.com/mhjabreel/CharCnn_Keras/master/data/ag_news_csv/test.csv"
test_df = pd.read_csv(test_url, header=None)
test_df.columns = ["label", "title", "description"]


import re, itertools
from collections import Counter

# 1-1) 簡易トークナイザー
def basic_tokenize(text: str):
    return re.findall(r"[A-Za-z]+|\d+|[^\sA-Za-z\d]", text.lower())

# 1-2) 語彙を上位 N 語で作る
vocab_size = 50000
counter = Counter()
for txt in itertools.chain(train_df["title"], train_df["description"]):
    counter.update(basic_tokenize(txt))
# 上位 (vocab_size–2) 語を取り出し、"<pad>","<unk>" を先頭に
itos = ["<pad>", "<unk>", "<cls>"] + [w for w,_ in counter.most_common(vocab_size-3)]
stoi = {w:i for i,w in enumerate(itos)}
cls_id = stoi["<cls>"]

max_len = 64

def encode(text: str):
    toks = basic_tokenize(text)[: max_len-1]      # CLS 分 を残す
    ids  = [stoi.get(t, 1) for t in toks]        # <unk>=1
    ids  = [cls_id] + ids                        # 先頭に CLS を追加
    ids += [0] * (max_len - len(ids))            # pad=0
    return ids



from torch.utils.data import Dataset, DataLoader
import torch

class NewsDataset(Dataset):
    def __init__(self, df):
        self.texts  = (df["title"] + " " + df["description"]).tolist()
        self.labels = (df["label"] - 1).tolist()         # 0inedx

    def __len__(self):
        return len(self.labels)

    def __getitem__(self, idx):
        input_ids = torch.tensor(encode(self.texts[idx]), dtype=torch.long)
        label     = torch.tensor(self.labels[idx], dtype=torch.long)
        return input_ids, label


from sklearn.model_selection import train_test_split

# train_df は 120 000 行
train_df_split, valid_df = train_test_split(
    train_df,
    test_size=0.1,              # 10 %
    stratify=train_df["label"], # ラベル分布を保つ
    random_state=42)



train_ds = NewsDataset(train_df_split)
valid_ds = NewsDataset(valid_df)
test_ds  = NewsDataset(test_df)

batch_size = 64
train_loader = DataLoader(train_ds, batch_size, shuffle=True)
valid_loader = DataLoader(valid_ds, batch_size)
test_loader  = DataLoader(test_ds , batch_size)


device = "cuda" if torch.cuda.is_available() else "cpu"

model = NewsTransformerClassifier(
    vocab_size=len(itos),
    num_labels=4,
    d_model=128,
    n_heads=4,
    num_layers=2,
    max_len=max_len
).to(device)

optimizer = torch.optim.AdamW(model.parameters(), lr=3e-4)
criterion = nn.CrossEntropyLoss()


def evaluate(loader):
    model.eval()
    tot, correct, tot_loss = 0, 0, 0.0
    with torch.no_grad():
        for ids, labels in loader:
            ids, labels = ids.to(device), labels.to(device)
            logits = model(ids)
            loss   = criterion(logits, labels)
            preds  = logits.argmax(dim=-1)

            tot_loss += loss.item() * labels.size(0)
            correct  += (preds == labels).sum().item()
            tot      += labels.size(0)
    return tot_loss / tot, correct / tot

train_losses = []
train_accs   = []
val_losses   = []
val_accs     = []

epochs = 10
for ep in range(1, epochs+1):
    running = 0.0
    for ids, labels in train_loader:
        ids, labels = ids.to(device), labels.to(device)

        optimizer.zero_grad()
        logits = model(ids)
        loss   = criterion(logits, labels)
        loss.backward()
        optimizer.step()

        running += loss.item() * labels.size(0)

    train_loss, train_acc = evaluate(train_loader)
    val_loss,   val_acc   = evaluate(valid_loader)

    # 3) ログと格納
    train_losses.append(train_loss)
    train_accs.append(train_acc)
    val_losses.append(val_loss)
    val_accs.append(val_acc)
    print(f"[Ep {ep}] train_loss {train_loss:.4f} |  train_acc {train_acc:.4f}"
          f"val_loss {val_loss:.4f} | val_acc {val_acc:.4f}")


# ── プロット ──
epochs_range = range(1, epochs+1)

plt.figure(figsize=(8,4))
plt.plot(epochs_range, train_losses, label='Train Loss')
plt.plot(epochs_range, val_losses,   label='Valid Loss')
plt.xlabel('Epoch'); plt.ylabel('Loss')
plt.legend(); plt.grid(True)
plt.show()

plt.figure(figsize=(8,4))
plt.plot(epochs_range, train_accs, label='Train Acc')
plt.plot(epochs_range, val_accs,   label='Valid Acc')
plt.xlabel('Epoch'); plt.ylabel('Accuracy')
plt.legend(); plt.grid(True)
plt.show()



test_loss, test_acc = evaluate(test_loader)
print(f"TEST  loss {test_loss:.4f} | acc {test_acc:.4f}")



