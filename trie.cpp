#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
using namespace std;

// Estrutura de nó para a Trie
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

// Inserir uma palavra na Trie
void insert(TrieNode* root, const string& word) {
    TrieNode* current = root;
    for (char ch : word) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = new TrieNode();
        }
        current = current->children[ch];
    }
    current->isEndOfWord = true;
}

// Buscar uma palavra na Trie
bool search(TrieNode* root, const string& word) {
    TrieNode* current = root;
    for (char ch : word) {
        if (current->children.find(ch) == current->children.end()) {
            return false;
        }
        current = current->children[ch];
    }
    return current->isEndOfWord;
}

// Remover uma palavra da Trie
bool remove(TrieNode* current, const string& word, int depth = 0) {
    if (!current) return false;

    if (depth == word.size()) {
        if (!current->isEndOfWord) return false;
        current->isEndOfWord = false;
        return current->children.empty();
    }

    char ch = word[depth];
    if (current->children.find(ch) != current->children.end() && 
        remove(current->children[ch], word, depth + 1)) {
        delete current->children[ch];
        current->children.erase(ch);
        return !current->isEndOfWord && current->children.empty();
    }

    return false;
}

// Exibir palavras armazenadas na Trie
void display(TrieNode* root, string& prefix) {
    if (root->isEndOfWord) {
        cout << prefix << endl;
    }

    for (auto& pair : root->children) {
        prefix.push_back(pair.first);
        display(pair.second, prefix);
        prefix.pop_back();
    }
}

// Gerar representação Graphviz para a Trie
void generateGraphviz(TrieNode* node, ofstream& file, int& nodeId, int parentId = -1, char edgeLabel = '\0') {
    int currentNodeId = nodeId++;
    file << "  node" << currentNodeId << " [label=\"";
    if (edgeLabel != '\0') file << edgeLabel;
    if (node->isEndOfWord) file << "*";
    file << "\"]\n";

    if (parentId != -1) {
        file << "  node" << parentId << " -> node" << currentNodeId << "\n";
    }

    for (auto& pair : node->children) {
        generateGraphviz(pair.second, file, nodeId, currentNodeId, pair.first);
    }
}

void saveGraphToFile(TrieNode* root, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo!\n";
        return;
    }

    file << "digraph G {\nnode [shape=circle];\n";

    int nodeId = 0;
    generateGraphviz(root, file, nodeId);

    file << "}\n";
    file.close();

    cout << "Arquivo DOT gerado: " << filename << endl;
}

int main() {
    TrieNode* root = new TrieNode();
    int choice;
    string word;

    while (true) {
        cout << "\n1. Inserir Palavra\n2. Buscar Palavra\n3. Remover Palavra\n4. Exibir Palavras\n5. Gerar Grafo\n6. Sair\nEscolha uma opção: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Digite a palavra para inserir: ";
                cin >> word;
                insert(root, word);
                break;
            case 2:
                cout << "Digite a palavra para buscar: ";
                cin >> word;
                if (search(root, word)) {
                    cout << "Palavra encontrada!\n";
                } else {
                    cout << "Palavra não encontrada.\n";
                }
                break;
            case 3:
                cout << "Digite a palavra para remover: ";
                cin >> word;
                if (remove(root, word)) {
                    cout << "Palavra removida com sucesso!\n";
                } else {
                    cout << "Palavra não encontrada ou não pôde ser removida.\n";
                }
                break;
            case 4:
                {
                    string prefix;
                    cout << "Palavras armazenadas na Trie:\n";
                    display(root, prefix);
                }
                break;
            case 5:
                saveGraphToFile(root, "trie.dot");
                break;
            case 6:
                cout << "Saindo...\n";
                return 0;
            default:
                cout << "Opção inválida.\n";
        }
    }
}
