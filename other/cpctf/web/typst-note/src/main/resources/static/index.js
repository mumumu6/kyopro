fetch("/notes/list.json").then(async (response) => {
  const { notes } = await response.json();
  const notesContainer = document.getElementById("notes-list");

  notes.forEach(({ id, title }) => {
    const noteLi = document.createElement("li");
    const noteLink = document.createElement("a");
    noteLink.href = `/notes/${id}.pdf`;
    noteLink.target = "_blank";
    noteLink.rel = "noopener noreferrer";
    noteLink.innerText = title;
    noteLi.appendChild(noteLink);
    notesContainer.appendChild(noteLi);
  });
});
