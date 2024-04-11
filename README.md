# E-Voting System based on Blockchain

## Overview
This project aims to create an electronic voting system leveraging blockchain technology. The system will utilize C++ for the blockchain implementation and Next.js for the web interface. The blockchain system will be built from scratch with P2P communication using Boost library. Cryptographic tools using OpenSSL library will be employed for ensuring the security and implementation of the blockchain system, including digital signatures and SHA256 hashing. 

## Components

### P2P Communication
The Peer-to-Peer (P2P) communication will allow nodes in the network to interact directly, enabling decentralized operation. Boost library will be utilized to establish P2P connections between nodes.

### Cryptographic Tools
1. **Digital Signature**: Digital signatures will be used to verify the authenticity of transactions and ensure that they have not been tampered with.
2. **SHA256 Hash**: SHA256 hashing will be employed for creating unique identifiers for each block in the blockchain, ensuring integrity and immutability + using for the POW.

### File Handling
1. **Keys Storage**: Private and public keys will be stored in PEM files for secure handling and access control.
2. **Blockchain Copy**: A copy of the blockchain will be saved in a CSV file for easy access and analysis.

## Usage
1. Clone the repository from GitHub.
2. Install necessary dependencies for both the C++ blockchain implementation and the Next.js web interface:
   - Next.js
   - OpenSSL
   - Boost
3. Run the web system using `npm run dev` command.
4. Run the blockchain system using the provided scripts.

## Disclaimer
This project is for educational and experimental purposes only. It should not be used for real-world applications without thorough testing and security audits.

