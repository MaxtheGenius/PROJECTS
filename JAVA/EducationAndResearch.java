import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/*
 * GenomicDataVisualizer.java
 * This Java application visualizes and analyzes genomic data, allowing researchers to explore DNA sequences.
 */
public class GenomicDataVisualizer {

    // Class that represents a DNA sequence
    private static class DNASequence {
        private String sequence;

        public DNASequence(String sequence) {
            this.sequence = sequence;
        }

        // Displays the DNA sequence
        public void displaySequence() {
            System.out.println("DNA Sequence: " + sequence);
        }

        // Analyzes the DNA sequence and counts occurrences of each base
        public void analyzeSequence() {
            int aCount = 0, tCount = 0, cCount = 0, gCount = 0;

            // Traverse the sequence and count the bases
            for (char base : sequence.toCharArray()) {
                switch (base) {
                    case 'A':
                        aCount++;
                        break;
                    case 'T':
                        tCount++;
                        break;
                    case 'C':
                        cCount++;
                        break;
                    case 'G':
                        gCount++;
                        break;
                    default: // Ignore invalid bases
                        System.out.println("Invalid base found: " + base);
                        break;
                }
            }
            // Display sequence analysis
            System.out.println("A: " + aCount);
            System.out.println("T: " + tCount);
            System.out.println("C: " + cCount);
            System.out.println("G: " + gCount);
        }

        // Transcribes the DNA sequence to RNA (replacing T with U)
        public void transcribeToRNA() {
            String rnaSequence = sequence.replace('T', 'U');
            System.out.println("RNA Sequence: " + rnaSequence);
        }

        // Translates the DNA sequence to a protein sequence (simplified)
        public void translateToProtein() {
            StringBuilder proteinSequence = new StringBuilder();
            for (int i = 0; i < sequence.length(); i += 3) {
                if (i + 2 < sequence.length()) {
                    String codon = sequence.substring(i, i + 3);
                    String aminoAcid = codonToAminoAcid(codon);
                    if (aminoAcid != null) {
                        proteinSequence.append(aminoAcid).append("-");
                    }
                }
            }
            System.out.println("Protein Sequence: " + proteinSequence.toString());
        }

        // Function that maps a codon (triplet of bases) to an amino acid (simplified)
        private String codonToAminoAcid(String codon) {
            switch (codon) {
                case "ATG":
                    return "M"; // Methionine
                case "TTT":
                    return "F"; // Phenylalanine
                case "TTC":
                    return "F"; // Phenylalanine
                case "TTA":
                    return "L"; // Leucine
                case "TTG":
                    return "L"; // Leucine
                // Add more mappings as necessary
                default:
                    return null; // Unmapped codon
            }
        }
    }

    // Class that handles user interaction
    private static class UserInterface {

        // Allows the user to choose which DNA sequence to load and which analysis to
        // perform
        public static void interact() {
            Scanner scanner = new Scanner(System.in);
            List<DNASequence> sequences = new ArrayList<>();

            // Display options to the user
            System.out.println("Welcome to the Genomic Data Visualizer.");
            System.out.println("Please enter a DNA sequence (or type 'exit' to quit):");

            // Enter DNA sequences
            while (true) {
                String input = scanner.nextLine().toUpperCase().trim();
                if (input.equals("EXIT")) {
                    break;
                } else if (isValidDNASequence(input)) {
                    sequences.add(new DNASequence(input));
                    System.out.println("DNA sequence added.");
                    System.out.println("Enter another sequence (or type 'exit' to quit):");
                } else {
                    System.out.println("Invalid DNA sequence. Please enter a valid sequence.");
                }
            }

            // Interaction to choose which analysis to perform
            for (DNASequence sequence : sequences) {
                sequence.displaySequence();
                System.out.println("Choose an action for this sequence:");
                System.out.println("1. Analyze sequence");
                System.out.println("2. Transcribe to RNA");
                System.out.println("3. Translate to Protein");

                int choice = scanner.nextInt();
                scanner.nextLine(); // Clear the buffer

                switch (choice) {
                    case 1:
                        sequence.analyzeSequence();
                        break;
                    case 2:
                        sequence.transcribeToRNA();
                        break;
                    case 3:
                        sequence.translateToProtein();
                        break;
                    default:
                        System.out.println("Invalid choice.");
                }
                System.out.println(); // New line
            }
        }

        // Verifies if the DNA sequence is valid
        private static boolean isValidDNASequence(String sequence) {
            return sequence.matches("[ATCG]+");
        }
    }

    // Main method that runs the user interface
    public static void main(String[] args) {
        UserInterface.interact(); // Start user interaction
    }
}