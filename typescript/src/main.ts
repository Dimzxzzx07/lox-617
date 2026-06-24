import { encryptText } from "./core";

const button =
    document.getElementById("enc") as HTMLButtonElement;

button.onclick = () => {
    const key =
        (document.getElementById("key") as HTMLInputElement).value;

    const text =
        (document.getElementById("text") as HTMLInputElement).value;

    const result =
        encryptText(key, text);

    (document.getElementById("result") as HTMLPreElement)
        .textContent = result;
};