import "@unocss/reset/sanitize/sanitize.css";
import "@unocss/reset/sanitize/forms.css";
import "@unocss/reset/sanitize/typography.css";
import "virtual:uno.css";

const inputPre = document.getElementById("input");
const equalBtn = document.getElementById("equal");
const historyPre = document.getElementById("history");

const pushHistory = async (inputText) => {
  const entrySpan = document.createElement("span");
  entrySpan.innerText = inputText;
  historyPre.appendChild(entrySpan);

  const equalSpan = document.createElement("span");
  equalSpan.classList.add("text-emerald-300");
  equalSpan.innerText = " = ";
  historyPre.appendChild(equalSpan);

  const resultSpan = document.createElement("span");
  resultSpan.classList.add("text-yellow-500");
  resultSpan.innerText = "(Calculating...)";
  historyPre.appendChild(resultSpan);

  historyPre.appendChild(document.createElement("br"));
  historyPre.scrollTop = historyPre.scrollHeight;

  try {
    const response = await fetch("/api/calc", {
      method: "POST",
      headers: { "Content-Type": "text/plain" },
      body: inputText,
    });
    const resultText = await response.text();
    if (!response.ok) throw new Error(resultText || response.statusText);

    resultSpan.classList.remove("text-yellow-500");
    resultSpan.innerText = resultText;
  } catch (error) {
    resultSpan.classList.remove("text-yellow-500");
    resultSpan.classList.add("text-rose-500");
    resultSpan.innerText = `(${error.message})`;
  }

  historyPre.scrollTop = historyPre.scrollHeight;
};

pushHistory('\"Hello, \" + \"World!\"');
pushHistory("`His BMI is ${60 / 170e-2 ** 2} kg/m^2`");

document.addEventListener("keydown", (e) => {
  if (e.key === "Enter" || e.key === "=") {
    equalBtn.click();
  } else if (e.key === "Backspace") {
    inputPre.innerText = inputPre.innerText.slice(0, -1);
  } else if (e.key.length === 1 && !e.ctrlKey && !e.metaKey) {
    if (inputPre.innerText.length < 64 && !/[()\[\].=]/.test(e.key)) {
      inputPre.innerText += e.key;
    }
  } else {
    return;
  }

  e.preventDefault();
});

equalBtn.addEventListener("click", () => {
  if (inputPre.innerText.trim() === "") return;
  pushHistory(inputPre.innerText);
  inputPre.innerText = "";
});
