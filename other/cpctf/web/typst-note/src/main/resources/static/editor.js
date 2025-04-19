import ace from "https://cdn.jsdelivr.net/npm/ace-builds@1.39.1/+esm";
import { $typst } from "https://cdn.jsdelivr.net/npm/@myriaddreamin/typst.ts/dist/esm/contrib/all-in-one-lite.bundle.js";

// Ace editor

const defaultContent = `
#set page(width: 10cm, height: auto)
#set heading(numbering: "1.")

= Fibonacci sequence
The Fibonacci sequence is defined through
the recurrence relation $F_n = F_(n-1) + F_(n-2)$.
It can also be expressed in _closed form:_

$ F_n = round(1 / sqrt(5) phi.alt^n), quad
  phi.alt = (l + sqrt(5)) / 2 $

#let count = 8
#let nums = range(1, count + 1)
#let fib(n) = (
  if n <= 2 { 1 }
  else { fib(n - 1) + fib(n - 2) }
)

The first #count numbers of the sequence are:

#align(center, table(
  columns: count,
  ..nums.map(n => $F_#n$),
  ..nums.map(n => str(fib(n))),
))
`.trimStart();

const editor = ace.edit("editor", {
  fontSize: 16,
  wrap: true,
  indentedSoftWrap: false,
  value: defaultContent,
});

// Request Publish button

const pageIdInput = document.getElementById("page-id-input");
const requestPublishButton = document.getElementById("request-publish");
const modal = document.getElementById("modal");

requestPublishButton.addEventListener("click", async () => {
  pageIdInput.value = pageIdInput.value.trim();
  if (pageIdInput.value.length === 0) {
    window.alert("Please enter a page ID.");
    return;
  }

  modal.classList.add("show");
  modal.firstElementChild.innerText = "Submitting Publish Request to Admin...";

  let succeeded = false;
  try {
    const response = await fetch("/api/request-publish", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ pageId: pageIdInput.value, content: editor.getValue() }),
    });
    if (response.ok) succeeded = true;
  } catch (error) {
    console.error(error);
  }

  modal.firstElementChild.classList.add(succeeded ? "accent" : "error");
  modal.firstElementChild.innerText = succeeded ? "Publish Request Submitted!" : "Failed to Submit Publish Request";
  setTimeout(() => {
    modal.firstElementChild.classList.remove("accent", "error");
    modal.classList.remove("show");
  }, 2000);
});

// Typst preview

$typst.setCompilerInitOptions({
  getModule: () =>
    "https://cdn.jsdelivr.net/npm/@myriaddreamin/typst-ts-web-compiler/pkg/typst_ts_web_compiler_bg.wasm",
});
$typst.setRendererInitOptions({
  getModule: () => "https://cdn.jsdelivr.net/npm/@myriaddreamin/typst-ts-renderer/pkg/typst_ts_renderer_bg.wasm",
});

const preview = document.getElementById("preview");
const message = document.getElementById("message");

const renderPreview = async () => {
  try {
    preview.innerHTML = await $typst.svg({ mainContent: editor.getValue() });
    const svg = preview.firstElementChild;
    svg.removeAttribute("height");
    svg.setAttribute("width", `100%`);
    message.classList.remove("error");
    message.innerText = "No errors";
  } catch (error) {
    message.classList.add("error");
    message.innerText = error;
  }
};

editor.session.on("change", async (_delta) => {
  await renderPreview();
});

renderPreview();
