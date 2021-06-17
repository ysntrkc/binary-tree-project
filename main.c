// Yasin Tarakçı

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// I defined a global variable to calculate the total access time.
// I defined it globally because we update it in a void function.
long totalAccTime = 0;

// Here, I define a struct for creating the trees.
typedef struct node {
    int number;
    char word[20];
    int frequency;
    struct node *left;      // This one for left child of the tree.
    struct node *right;     // This one for right child of the tree.
} node;

// This function creates the nodes one by one with the given id, word and freq.
struct node *createNode(int id, char *word, int freq) {
    struct node *newNode;
    newNode = malloc(sizeof(struct node));      // Here, I allocate the memory for the new node.
    newNode->number = id;
    strcpy(newNode->word, word);
    newNode->frequency = freq;
    newNode->left = newNode->right = NULL;      // At first, when I create a new node it's left and right nodes are NULL.
    return newNode;
}

// This function builds the binary search tree by comparing the words alphabetically.
struct node *insertToWordBST(struct node *currentNode, int id, char *word, int freq) {
    // If the current node is empty, I create a new node with the given variables.
    if (currentNode == NULL) {
        return createNode(id, word, freq);
    }
        // If the current node is not empty, I compare the words and send the node to the right or left.
    else if (stricmp(word, currentNode->word) < 0) {
        currentNode->left = insertToWordBST(currentNode->left, id, word, freq);
    } else if (stricmp(word, currentNode->word) >= 0) {
        currentNode->right = insertToWordBST(currentNode->right, id, word, freq);
    }
    // When there is no node then I return the updated node.
    return currentNode;
}

// This function builds a binary tree by using the sorted node array.
struct node *insertToFreqBT(struct node *nodeArr, struct node *currentNode, int i, int lineCount) {
    // Actually the lineCount is equal to array's length so I can use it.
    if (i < lineCount) {
        currentNode = &nodeArr[i];

        // With these recursions, I insert the nodes to a binary tree from the root to the deepest leaves.
        currentNode->left = insertToFreqBT(nodeArr, currentNode->left, 1 + i * 2, lineCount);
        currentNode->right = insertToFreqBT(nodeArr, currentNode->right, 2 + i * 2, lineCount);
    }
    // When there is no node then I return the updated node.
    return currentNode;
}

// This function prints the tree pre-order traversal.
void printPreOrderNLR(struct node *currentNode) {
    if (currentNode != NULL) {
        printf("|%-15s%-4s", currentNode->word, "->");
        printPreOrderNLR(currentNode->left);
        printPreOrderNLR(currentNode->right);
    }
}

// This function prints the tree in-order traversal.
void printInOrderLNR(struct node *currentNode) {
    if (currentNode != NULL) {
        printInOrderLNR(currentNode->left);
        printf("|%-15s%-4s", currentNode->word, "->");
        printInOrderLNR(currentNode->right);
    }
}

// This function prints the tree reverse in-order traversal.
void printReverseInorderRNL(struct node *currentNode) {
    if (currentNode != NULL) {
        printReverseInorderRNL(currentNode->right);
        printf("|%-15s%-4s", currentNode->word, "->");
        printReverseInorderRNL(currentNode->left);
    }
}

// This function prints the tree post-order traversal.
void printPostOrderLRN(struct node *currentNode) {
    if (currentNode != NULL) {
        printPostOrderLRN(currentNode->left);
        printPostOrderLRN(currentNode->right);
        printf("|%-15s%-4s", currentNode->word, "->");
    }
}

// This function calculates the total access time of the tree by node's depth and frequency.
void totalAccessTime(struct node *currentNode, int depth) {
    if (currentNode == NULL) {
        // When the node return back to the root I update the depth to 0.
        depth = 0;
    } else {
        // When the node go down, depth is incremented by 1.
        totalAccessTime(currentNode->left, depth + 1);
        totalAccessTime(currentNode->right, depth + 1);

        // Here I calculate and update the totalAccTime.
        totalAccTime += (depth + 1) * currentNode->frequency;
    }
}


int main() {
    int nodeId;
    int nodeFreq;
    int max, i, j;
    int lineCount = 0;
    char line[30] = "\0";
    char nodeWord[20] = "\0";
    char nodeIdChar[3] = "\0";
    char nodeFreqChar[5] = "\0";
    struct node *freqOrderBT;
    struct node *wordOrderedBST;
    unsigned int choice1, choice2;

    // Here, I open the file for the first time to read it.
    FILE *filePtr = fopen("input.txt", "r");

    // If there is no file, the program prints a message and returns 1.
    if (filePtr == NULL) {
        printf("File couldn't be opened!\n");
        return 1;
    }

    // If there is no problem, the program starts to read the file and insert the nodes into the tree.
    // If there is a line to read, the program reads it and assigns the line variable.
    while (fgets(line, sizeof(line), filePtr)) {

        // With strtok() function, I split the 'line' from semicolons and copy them to given string variables.
        strcpy(nodeIdChar, strtok(line, ","));
        strcpy(nodeWord, strtok(NULL, ","));
        strcpy(nodeFreqChar, strtok(NULL, ","));

        // With the atoi() function, I convert strings to integers.
        nodeId = strtol(nodeIdChar, (char**) NULL, 10);
        nodeFreq = strtol(nodeFreqChar, (char**) NULL, 10);

        // If I reading the first line of the file, I have to create a root.
        if (lineCount == 0) {
            wordOrderedBST = createNode(nodeId, nodeWord, nodeFreq);
        }
            // If I reading the other lines, I insert the nodes to root for build a binary search tree.
        else {
            insertToWordBST(wordOrderedBST, nodeId, nodeWord, nodeFreq);
        }
        // Every line which I read, the lineCount incremented by 1.
        lineCount++;
    }
    // When there is no line to read, I close the file.
    fclose(filePtr);

    // Here, I open the file for the second time to read it.
    filePtr = fopen("input.txt", "r");

    if (filePtr == NULL) {
        printf("File couldn't be opened!\n");
        return 1;
    }

    // Here, I create a node array for store all nodes and sort them according to their frequency.
    struct node nodeArr[lineCount];

    // This for loop reads the file line by line. After read line create the nodes and store them into the node array.
    for (i = 0; fgets(line, sizeof(line), filePtr); i++) {
        strcpy(nodeIdChar, strtok(line, ","));
        strcpy(nodeWord, strtok(NULL, ","));
        strcpy(nodeFreqChar, strtok(NULL, ","));

        nodeId = strtol(nodeIdChar, (char**) NULL, 10);
        nodeFreq = strtol(nodeFreqChar, (char**) NULL, 10);

        nodeArr[i] = *createNode(nodeId, nodeWord, nodeFreq);
    }

    // With this for loop, I sort the nodes by frequency.
    // For sorting nodes, the program swaps the nodes one by one.
    struct node tempNode;
    for (i = 0; i < lineCount - 1; i++) {
        max = i;
        for (j = i + 1; j < lineCount; j++) {
            if (nodeArr[j].frequency > nodeArr[max].frequency) {
                max = j;
            }
        }
        tempNode = nodeArr[i];
        nodeArr[i] = nodeArr[max];
        nodeArr[max] = tempNode;
    }
    fclose(filePtr);

    // The root of our second tree is the node that has the highest frequency and that is the first one of the array.
    freqOrderBT = &nodeArr[0];
    insertToFreqBT(nodeArr, freqOrderBT, 0, lineCount);

    // This part of the program is for user experience.
    // Firstly, I ask to the user to use which tree.
    printf("Which tree do you want to use?\n\tType 1 for word ordered binary search tree."
           "\n\tType 2 for level ordered binary tree.\n?");
    scanf("%d", &choice1);

    // There is only two options. So if user entered any other option, the program prints an error message and exits.
    if (!(choice1 == 1 || choice1 == 2)) {
        puts("You entered an unavailable option!");
        return 1;
    }

    // Second, I ask to the user to do what with the selected tree.
    printf("What do you want to print?\n\tType 1 for pre-order traversal (NLR)"
           "\n\tType 2 for post-order traversal (LRN)\n\tType 3 for in order traversal (LNR)"
           "\n\tType 4 for reverse in order traversal (RNL)\n\tType 5 for calculate the total access time.\n?");
    scanf("%d", &choice2);

    // There is only five options. So if user entered any other option, the program prints an error message and exits.
    if (!(choice2 == 1 || choice2 == 2 || choice2 == 3 || choice2 == 4 || choice2 == 5)) {
        puts("You entered an unavailable option!");
        return 1;
    }

    // Finally, the program do whatever user wants and print it to the console.
    if (choice1 == 1) {
        if (choice2 == 1) printPreOrderNLR(wordOrderedBST);
        else if (choice2 == 2) printPostOrderLRN(wordOrderedBST);
        else if (choice2 == 3) printInOrderLNR(wordOrderedBST);
        else if (choice2 == 4) printReverseInorderRNL(wordOrderedBST);
        else if (choice2 == 5) {
            totalAccessTime(wordOrderedBST, 0);
            printf("Total access time: %ld", totalAccTime);
        }
    } else if (choice1 == 2) {
        if (choice2 == 1) printPreOrderNLR(freqOrderBT);
        else if (choice2 == 2) printPostOrderLRN(freqOrderBT);
        else if (choice2 == 3) printInOrderLNR(freqOrderBT);
        else if (choice2 == 4) printReverseInorderRNL(freqOrderBT);
        else if (choice2 == 5) {
            totalAccessTime(freqOrderBT, 0);
            printf("Total access time: %ld", totalAccTime);
        }
    }

    return 0;
}
