from flask import Flask, request, render_template_string
import hashlib

css = """
<style>
    body {
        font-family: 'Helvetica Neue', Arial, sans-serif;
        background-color: #f5f5f5;
        color: #333;
        max-width: 800px;
        margin: 0 auto;
        padding: 20px;
        text-align: center;
    }
    h1 {
        color: #8e44ad;
        margin-bottom: 30px;
    }
    form {
        background-color: white;
        padding: 20px;
        border-radius: 10px;
        box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        margin-bottom: 20px;
    }
    input[type="text"] {
        padding: 10px;
        width: 300px;
        border: 1px solid #ddd;
        border-radius: 5px;
        margin-right: 10px;
    }
    input[type="submit"] {
        background-color: #8e44ad;
        color: white;
        border: none;
        padding: 10px 20px;
        border-radius: 5px;
        cursor: pointer;
    }
    input[type="submit"]:hover {
        background-color: #732d91;
    }
    .result {
        background-color: white;
        padding: 20px;
        border-radius: 10px;
        box-shadow: 0 2px 10px rgba(0,0,0,0.1);
    }
    .fortune {
        font-size: 24px;
        font-weight: bold;
        color: #e74c3c;
    }
    .flag {
        margin-top: 20px;
        padding: 10px;
        background-color: #f1c40f;
        border-radius: 5px;
    }
</style>
"""


app = Flask(__name__)


def get_fortune(name):
    hash_value = hashlib.sha256(name.encode()).hexdigest()
    zero_count = hash_value.count("0")

    if zero_count < 4:
        return "大凶"
    elif zero_count < 8:
        return "凶"
    elif zero_count < 16:
        return "小吉"
    elif zero_count < 32:
        return "中吉"
    elif zero_count < 64:
        return "吉"
    else:
        return "大吉"


@app.route("/")
def index():
    name = request.args.get("name")
    if name is None:
        return f"""
        {css}
        <h1>🔮 名前占い 🔮</h1>
        <form action="/" method="get">
            <label for="name">あなたの名前を入力してください:</label>
            <input type="text" id="name" name="name" required>
            <input type="submit" value="占う">
        </form>
        """

    fortune = get_fortune(name)

    result = f"""
    {css}
    <h1>🔮 名前占い 🔮</h1>
    <div class="result">
        <p>こんにちは、{name}さん。</p>
        <p>あなたの運勢は…… <span class="fortune">{fortune}</span> です！</p>
    """

    if fortune == "大吉":
        with open("flag.txt", "r") as f:
            content = f.read()
            result += f'<div class="flag">フラグは{content}です。</div>'

    result += "</div>"
    return render_template_string(result)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=False)  # ← host を明示

