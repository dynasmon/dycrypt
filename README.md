# DyCrypt

A powerful and educational cryptographic project implemented in C, featuring Caesar Cipher encryption and a digital authentication system using AVL trees. This project demonstrates data encryption, decryption, and document integrity verification using hashing.

---

## Features

- **Encryption and Decryption** with Caesar Cipher using a doubly linked list for text storage.
- **Digital Authentication** using an AVL tree to organize document lines lexicographically and generate an authentication code with SHA-1 hashing.

---

## Dependencies

To build and run DyCrypt, you need the following dependencies installed on your Linux system:

1. Install the C Compiler (GCC) and OpenSSL:
   ```bash
   sudo apt-get install build-essential libssl-dev
   ```

## Installation

Clone the repository:

```bash
git clone https://github.com/dycrypt/dycrypt.git
cd dycrypt
```

Compile the source code:

```bash
gcc crypt.c -o crypt -lssl -lcrypto
```

Run the program:

```bash
./crypt
```

## Usage

Upon running the program, you will be prompted to choose between different operations:

- **Encrypt a Message**:
  - Input your message and provide a shift value to encrypt using the Caesar Cipher.
  - Example input: "Hello World" with shift 3.

- **Decrypt a Message**:
  - Input an encrypted message and provide the shift value to decrypt it.
  - Example: Decrypt the previously encrypted "Khoor Zruog" with shift 3.

- **Authenticate a Document**:
  - Enter a document separated by semicolons (;), and the program will generate a root hash as the authentication code.
  - Example: "First line;Second line;Third line".

- **Verify Document Authenticity**:
  - Re-enter the document and provide the original hash to verify if the document is authentic.
  - The program will compare the root hash with the provided hash to determine if any changes were made.

## Troubleshooting

- **SHA-1 Header File Not Found**:
  - Make sure OpenSSL development libraries are installed:
  
  ```bash
  sudo apt-get install libssl-dev
  ```

- **Compilation Issues**:
  - Verify that the OpenSSL library is linked correctly during compilation:
  
  ```bash
  gcc crypt.c -o crypt -lssl -lcrypto
  ```

- **Running the Program**:
  - Ensure you have execution permissions for the compiled file:
  
  ```bash
  chmod +x crypt
  ./crypt
  ```

## License

This project is distributed under the MIT License. See the LICENSE file for details.
