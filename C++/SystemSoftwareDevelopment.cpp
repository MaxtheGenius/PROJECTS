#include <iostream>
#include <vector>
#include <algorithm>

using namespace std; // Use the standard namespace to simplify code readability

// Structure representing a block of memory
struct MemoryBlock {
    size_t start;      // Starting address of the block
    size_t size;       // Size of the block in bytes
    bool isAllocated;  // Status: true if allocated, false otherwise
};

// Class implementing the memory manager
class MemoryManager {
private:
    vector<MemoryBlock> memoryPool; // Vector holding the memory pool blocks
    size_t totalMemory;             // Total memory available for allocation

public:
    // Constructor: Initializes the memory manager with a specified total size
    MemoryManager(size_t totalSize) : totalMemory(totalSize) {
        // Initially, all memory is free and represented by a single block
        memoryPool.push_back({0, totalSize, false});
    }

    // Allocates memory of the specified size
    void* allocate(size_t size) {
        // Iterate through memory blocks to find a free block that fits the size
        for (auto& block : memoryPool) {
            if (!block.isAllocated && block.size >= size) {
                size_t remainingSize = block.size - size; // Calculate leftover space

                // Update the block to mark it as allocated
                block.isAllocated = true;
                block.size = size;

                // If there is leftover space, create a new block for it
                if (remainingSize > 0) {
                    MemoryBlock newBlock = {block.start + size, remainingSize, false};
                    memoryPool.insert(find(memoryPool.begin(), memoryPool.end(), block) + 1, newBlock);
                }

                // Return the starting address of the allocated block
                cout << "Allocated " << size << " bytes at address " << block.start << endl;
                return reinterpret_cast<void*>(block.start);
            }
        }

        // Allocation failed: Not enough memory available
        cout << "Allocation failed. Not enough memory available." << endl;
        return nullptr;
    }

    // Deallocates memory at a given address
    void deallocate(void* address) {
        size_t addr = reinterpret_cast<size_t>(address); // Convert address to size_t

        // Find the block with the matching starting address
        for (auto& block : memoryPool) {
            if (block.start == addr && block.isAllocated) {
                block.isAllocated = false; // Mark the block as free
                mergeFreeBlocks();         // Merge adjacent free blocks to reduce fragmentation
                cout << "Deallocated memory at address " << addr << endl;
                return;
            }
        }

        // Deallocation failed: Address not found or already free
        cout << "Deallocation failed. Address not found or already free." << endl;
    }

    // Displays the current layout of the memory pool
    void displayMemoryLayout() {
        cout << "Memory Layout:" << endl;
        for (const auto& block : memoryPool) {
            cout << "[" << block.start << " - " << block.start + block.size - 1 << "] ";
            cout << (block.isAllocated ? "Allocated" : "Free") << " (" << block.size << " bytes)" << endl;
        }
    }

private:
    // Merges adjacent free memory blocks to reduce fragmentation
    void mergeFreeBlocks() {
        for (size_t i = 0; i < memoryPool.size() - 1; ++i) {
            if (!memoryPool[i].isAllocated && !memoryPool[i + 1].isAllocated) {
                // Combine the sizes of the current and next blocks
                memoryPool[i].size += memoryPool[i + 1].size;
                memoryPool.erase(memoryPool.begin() + i + 1); // Remove the next block
                --i; // Adjust the index to recheck the merged block
            }
        }
    }
};

// Main function to demonstrate the memory manager
int main() {
    // Create a memory manager instance with 1024 bytes of memory
    MemoryManager memoryManager(1024);

    // Display the initial memory layout
    memoryManager.displayMemoryLayout();

    // Allocate memory blocks
    void* block1 = memoryManager.allocate(100);
    void* block2 = memoryManager.allocate(200);

    // Display the updated memory layout
    memoryManager.displayMemoryLayout();

    // Deallocate the first allocated block
    memoryManager.deallocate(block1);

    // Display the memory layout after deallocation
    memoryManager.displayMemoryLayout();

    // Attempt to allocate another block
    void* block3 = memoryManager.allocate(50);

    // Display the final memory layout
    memoryManager.displayMemoryLayout();

    return 0;
}