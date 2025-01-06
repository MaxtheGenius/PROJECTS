import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/*
 * SupplyChainBlockchain.java
 * Implements a simple blockchain application to track product provenance in a supply chain.
 */
public class SupplyChainBlockchain {

    // Step 1: Define the Block class
    /*
     * Represents a single block in the blockchain.
     * Each block contains:
     * - Index: Position of the block in the chain.
     * - Timestamp: Time of block creation.
     * - Data: Transaction data related to the block.
     * - Previous Hash: Hash of the previous block.
     * - Current Hash: Hash of this block.
     */
    static class Block {
        private final int index;
        private final long timestamp;
        private final String data;
        private final String previousHash;
        private final String currentHash;

        // Constructor for the Block class
        public Block(int index, long timestamp, String data, String previousHash) {
            this.index = index;
            this.timestamp = timestamp;
            this.data = data;
            this.previousHash = previousHash;
            this.currentHash = calculateHash();
        }

        // Method to calculate the hash of this block
        private String calculateHash() {
            String input = index + timestamp + data + previousHash;
            return applySHA256(input);
        }

        // Getter for the current hash
        public String getCurrentHash() {
            return currentHash;
        }

        // Getter for the previous hash
        public String getPreviousHash() {
            return previousHash;
        }

        // Format the block's details into a readable string
        @Override
        public String toString() {
            return "Block #" + index + "\n" +
                    "Timestamp: " + new Date(timestamp) + "\n" +
                    "Data: " + data + "\n" +
                    "Previous Hash: " + previousHash + "\n" +
                    "Current Hash: " + currentHash + "\n";
        }
    }

    // Step 2: Define the Blockchain class
    /*
     * Manages the blockchain by storing and validating blocks.
     */
    static class Blockchain {
        private final List<Block> chain;

        // Constructor to initialize the blockchain with the genesis block
        public Blockchain() {
            chain = new ArrayList<>();
            chain.add(createGenesisBlock());
        }

        // Creates the first block in the blockchain (genesis block)
        private Block createGenesisBlock() {
            return new Block(0, new Date().getTime(), "Genesis Block", "0");
        }

        // Adds a new block to the blockchain
        public void addBlock(String data) {
            Block lastBlock = chain.get(chain.size() - 1);
            Block newBlock = new Block(chain.size(), new Date().getTime(), data, lastBlock.getCurrentHash());
            chain.add(newBlock);
        }

        // Validates the integrity of the blockchain
        public boolean validateBlockchain() {
            for (int i = 1; i < chain.size(); i++) {
                Block currentBlock = chain.get(i);
                Block previousBlock = chain.get(i - 1);

                // Check the hash of the current block
                if (!currentBlock.getCurrentHash().equals(currentBlock.calculateHash())) {
                    return false;
                }

                // Check the link between the current and previous blocks
                if (!currentBlock.getPreviousHash().equals(previousBlock.getCurrentHash())) {
                    return false;
                }
            }
            return true;
        }

        // Displays the details of all blocks in the blockchain
        public void displayBlockchain() {
            for (Block block : chain) {
                System.out.println(block);
            }
        }
    }

    // Step 3: Define the SHA-256 hashing utility
    /*
     * Generates a secure hash using the SHA-256 algorithm.
     */
    private static String applySHA256(String input) {
        try {
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            byte[] hashBytes = digest.digest(input.getBytes("UTF-8"));
            StringBuilder hexString = new StringBuilder();
            for (byte b : hashBytes) {
                String hex = Integer.toHexString(0xff & b);
                if (hex.length() == 1)
                    hexString.append('0');
                hexString.append(hex);
            }
            return hexString.toString();
        } catch (Exception e) {
            throw new RuntimeException("Error applying SHA-256 hashing", e);
        }
    }

    // Step 4: Main method to run the application
    public static void main(String[] args) {
        // Initialize the blockchain
        Blockchain blockchain = new Blockchain();

        // Add blocks representing product transactions
        blockchain.addBlock("Product A: Manufactured at Factory X");
        blockchain.addBlock("Product A: Shipped to Warehouse Y");
        blockchain.addBlock("Product A: Delivered to Retailer Z");

        // Display the blockchain details
        System.out.println("=== Blockchain Details ===");
        blockchain.displayBlockchain();

        // Validate the blockchain's integrity
        boolean isValid = blockchain.validateBlockchain();
        System.out.println("Blockchain Integrity Valid: " + isValid);
    }
}