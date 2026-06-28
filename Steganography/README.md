# Image Steganography using Least Significant Bit (LSB) Technique in C

## 📌 Overview

This project implements **Image Steganography** using the **Least Significant Bit (LSB)** technique in the C programming language. The application securely hides a secret file inside a **24-bit BMP image** and later retrieves it without visibly altering the image.

The project demonstrates concepts such as file handling, bit manipulation, BMP image structure, dynamic memory management, and modular programming.

---

## ✨ Features

* Hide any secret file inside a BMP image
* Extract the hidden file from the encoded image
* Supports any secret file type by preserving the original file extension
* Validates input files before processing
* Maintains the visual appearance of the cover image
* Modular implementation using separate source and header files
* Efficient bit-level encoding and decoding

---

## 🛠 Technologies Used

* Programming Language: C
* Operating System: Linux
* Compiler: GCC
* Concepts:

  * File Handling
  * Bit Manipulation
  * Dynamic Memory Allocation
  * BMP Image Format
  * Modular Programming

---

## 📁 Project Structure

```text
Image-Steganography/
│
├── main.c
├── encode.c
├── decode.c
├── common.c
│
├── encode.h
├── decode.h
├── common.h
│
├── Makefile
├── README.md
├── LICENSE
├── .gitignore
│
├── images/
│   ├── input.bmp
│   └── encoded.bmp
│
└── secret/
    └── secret.txt
```

---

## 🧠 Working Principle

The project uses the **Least Significant Bit (LSB)** method to hide information.

Each byte of the secret data is divided into individual bits.

Each bit is embedded into the least significant bit of one image byte.

Since only the last bit changes, the difference is almost impossible to notice visually.

### Encoding Flow

```
Secret File
      │
      ▼
Read Secret Data
      │
      ▼
Convert Data into Bits
      │
      ▼
Replace LSB of BMP Pixel Bytes
      │
      ▼
Encoded BMP Image
```

### Decoding Flow

```
Encoded BMP Image
        │
        ▼
Read LSB of Image Bytes
        │
        ▼
Reconstruct Original Bytes
        │
        ▼
Recover Secret File
```

---

## 📦 Information Stored Inside the Image

The encoder stores the following information sequentially:

1. Magic String
2. Secret File Extension Size
3. Secret File Extension
4. Secret File Size
5. Secret File Data

This metadata allows the decoder to correctly identify and reconstruct the hidden file.

---

## 🚀 Compilation

Compile the project using:

```bash
gcc *.c
```

---

## ▶️ Usage

### Encoding

```bash
./a.out -e input.bmp secret.txt encoded.bmp
```

### Decoding

```bash
./a.out -d encoded.bmp
```

---

## 📚 Algorithm

### Encoding

1. Validate command-line arguments.
2. Open source image and secret file.
3. Verify BMP format.
4. Check whether the image has sufficient capacity.
5. Copy BMP header.
6. Encode:

   * Magic String
   * Extension Size
   * Extension
   * Secret File Size
   * Secret File Data
7. Copy remaining image data.
8. Generate the encoded image.

### Decoding

1. Validate command-line arguments.
2. Open encoded image.
3. Verify Magic String.
4. Decode extension size.
5. Decode extension.
6. Decode secret file size.
7. Recover secret file data.
8. Reconstruct the original file.

---

## ⏱ Time Complexity

### Encoding

* Time Complexity: **O(n)**
* Space Complexity: **O(1)** (excluding file storage)

### Decoding

* Time Complexity: **O(n)**
* Space Complexity: **O(1)**

---

## ⚠ Limitations

* Supports only **24-bit BMP** images.
* Does not encrypt the secret data.
* No password protection.
* Stores only one secret file at a time.
* Capacity depends on the size of the cover image.

---

## 🚀 Future Improvements

* Support PNG images
* AES encryption before embedding
* Password-protected decoding
* Compression before encoding
* Multiple secret file support
* GUI application
* Audio and video steganography

---

## 🎯 Learning Outcomes

Through this project, I gained practical experience in:

* File handling in C
* Bitwise operations
* Binary data manipulation
* BMP image structure
* Modular software design
* Memory management
* Command-line application development
* Error handling and validation

---

## 👨‍💻 Author

**Sanketh TS**

This project was developed as part of my learning in C programming, focusing on low-level programming concepts, data hiding techniques, and file manipulation.

## Screenshots

### Encoding

![Encoding](<Screenshot Encoding.png>)

### Decoding

![Decoding](<Screenshot Decoding.png>)



