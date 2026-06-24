import { encryptText } from "./core.js";

document
    .getElementById("enc")
    .onclick = () => {

    const key =
        document.getElementById("key").value;

    const text =
        document.getElementById("text").value;

    document
        .getElementById("result")
        .textContent =
            encryptText(key, text);
};