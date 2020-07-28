/**
 * @type {HTMLInputElement}
 */
let fileSelectButton = document.getElementById("openFile");

fileSelectButton.oninput = (event) => {
    /** @type {File} */
    let file = fileSelectButton.files[0];
    let reader = new FileReader();
    reader.onload = (e) =>
    {
        document.getElementById("sdlEffects").innerText = e.target.result;
    }
    reader.readAsText(file);
};