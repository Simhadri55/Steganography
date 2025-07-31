# 🕵️‍♂️ Steganography: Secret File Encoder & Decoder in BMP Images

A C project to **hide (encode)** and **extract (decode)** secret files within `.bmp` image files using **Least Significant Bit (LSB) Steganography**.

## 📌 Features

- Encode any secret file into a 24-bit BMP image.
- Decode and recover the original secret file from the stego image.
- Magic string verification to ensure encoded files are detected correctly.
- Interactive prompts to name decoded files or display data in terminal.
- Error handling for invalid inputs and file formats.

---

## 🗂️ Project Structure

```
.
├── encode.h             # Header for encoding operations
├── encode.c             # Implementation of encoding logic
├── decode.h             # Header for decoding operations
├── decode.c             # Implementation of decoding logic
├── types.h              # Common types and macros used
├── main.c               # Entry point for encode/decode execution
├── README.md            # This file
├── Makefile             # Build script (optional)
└── sample.bmp           # Sample cover image (for testing)
```

---

## 🚀 Getting Started

### 🔧 Requirements

- GCC Compiler
- Unix/Linux environment (Windows via WSL or MinGW is fine)
- 24-bit BMP image (as cover file)
- Any file as the secret

### 🔨 Compilation

```bash
gcc main.c encode.c decode.c -o steganography
```

### ▶️ Usage

#### To Encode:

```bash
./steganography -e <source_image.bmp> <secret_file> <output_image.bmp>
```

#### To Decode:

```bash
./steganography -d <stego_image.bmp>
```

You will be prompted for:
- File name to save the secret file (optional)
- Whether to display the extracted data in terminal

---

## 📝 Example

### Encoding:

```bash
./steganography -e image.bmp secret.txt output.bmp
```

**Output:**
```
File opened successfully
Encode magic string successfully
Encode secret file extension size successfully
...
Encoding completed successfully
```

### Decoding:

```bash
./steganography -d output.bmp
```

**Output:**
```
Do you want to name the secret file(y/n)? y
Enter name: recovered.txt
Do you want to print the data in terminal(y/n)? n
Decoded successfully
```

---

## 📷 What is LSB Steganography?

Least Significant Bit (LSB) steganography works by modifying the last bit of each pixel byte (RGB) to embed data. Since human vision is not sensitive to minor color changes, the secret remains imperceptible in the image.

---

## 🔒 Security Notes

- This project **does not use encryption**, only steganography.
- For secure data hiding, combine this with encryption.

---

## 🙋‍♂️ Author

**T Simhadri**  
🔗 [LinkedIn](https://www.linkedin.com/in/thondlam-simhadri)

---

## 📃 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## ⭐ Acknowledgements

- Developed as part of academic exploration at Emertxe Information Technologies.
- Inspired by the need for secure and invisible data hiding techniques.
