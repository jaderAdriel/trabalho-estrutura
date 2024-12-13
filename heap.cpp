#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

// Estrutura de nó para a árvore Heap
struct Node {
    int value;
    Node* left;
    Node* right;
};

// Função auxiliar para criar um novo nó
Node* createNode(int value) {
    return new Node{value, nullptr, nullptr};
}

// Função auxiliar para comparar valores (para Max-Heap e Min-Heap)
bool compare(int a, int b, bool isMinHeap) {
    return isMinHeap ? a < b : a > b;
}

// Função auxiliar para fazer o "heapify-down"
void heapifyDown(Node* node, bool isMinHeap) {
    if (!node || (!node->left && !node->right)) return;

    Node* extreme = node;

    if (node->left && compare(node->left->value, extreme->value, isMinHeap)) {
        extreme = node->left;
    }

    if (node->right && compare(node->right->value, extreme->value, isMinHeap)) {
        extreme = node->right;
    }

    if (extreme != node) {
        swap(node->value, extreme->value);
        heapifyDown(extreme, isMinHeap);
    }
}

// Função auxiliar para fazer o "heapify-up"
void heapifyUp(vector<int>& heap, int index, bool isMinHeap) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (compare(heap[index], heap[parentIndex], isMinHeap)) {
            swap(heap[index], heap[parentIndex]);
            index = parentIndex;
        } else {
            break;
        }
    }
}

// Inserir na Heap
Node* insert(Node* root, int value, queue<Node*>& nodes, bool isMinHeap) {
    Node* newNode = createNode(value);

    if (!root) {
        nodes.push(newNode);
        return newNode;
    }

    // Inserção no nó disponível da fila
    Node* parent = nodes.front();
    if (!parent->left) {
        parent->left = newNode;
    } else if (!parent->right) {
        parent->right = newNode;
        nodes.pop(); // Remove o nó que já tem dois filhos
    }

    nodes.push(newNode); // Adiciona o novo nó à fila

    return root;
}

// Remover o extremo (menor no Min-Heap ou maior no Max-Heap)
Node* removeExtreme(Node* root, bool isMinHeap) {
    if (!root) return nullptr;

    if (!root->left && !root->right) {
        delete root;
        return nullptr;
    }

    // Encontrar o nó mais à direita na última camada
    queue<Node*> q;
    q.push(root);
    Node* lastNode = nullptr;
    Node* parent = nullptr;

    while (!q.empty()) {
        lastNode = q.front();
        q.pop();

        if (lastNode->left) {
            parent = lastNode;
            q.push(lastNode->left);
        }

        if (lastNode->right) {
            parent = lastNode;
            q.push(lastNode->right);
        }
    }

    // Substituir a raiz pelo último nó
    root->value = lastNode->value;
    if (parent->right == lastNode) {
        parent->right = nullptr;
    } else {
        parent->left = nullptr;
    }

    delete lastNode;

    // Reequilibrar a heap
    heapifyDown(root, isMinHeap);

    return root;
}

// Função auxiliar para fazer heapify em toda a árvore
void heapify(Node* root, bool isMinHeap) {
    if (!root) return;

    // Realizar heapify em uma travessia pós-ordem
    if (root->left) heapify(root->left, isMinHeap);
    if (root->right) heapify(root->right, isMinHeap);

    heapifyDown(root, isMinHeap);
}

// Percurso em nível
void levelOrder(Node* node) {
    if (!node) return;

    queue<Node*> q;
    q.push(node);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        cout << current->value << " ";

        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
    cout << endl;
}

// Função para imprimir a árvore no formato Graphviz
void generateGraphviz(Node* node, ofstream& file, int& nodeId) {
    if (!node) return;

    file << "  node" << nodeId << " [label=\"" << node->value << "\"]\n";

    int currentNodeId = nodeId;

    if (node->left) {
        file << "  node" << currentNodeId << " -> node" << ++nodeId << "\n";
        generateGraphviz(node->left, file, nodeId);
    }

    if (node->right) {
        file << "  node" << currentNodeId << " -> node" << ++nodeId << "\n";
        generateGraphviz(node->right, file, nodeId);
    }
}

void saveGraphToFile(Node* root, const string& filename) {
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
    Node* root = nullptr;
    queue<Node*> nodes;
    int choice, key;
    bool isMinHeap;

    cout << "Escolha o tipo de heap: (1 para Min-Heap, 0 para Max-Heap): ";
    cin >> isMinHeap;

    while (true) {
        cout << "\n1. Inserir\n2. Remover Extremo\n3. Percurso Nível\n4. Gerar Grafo\n5. Heapify\n6. Sair\nEscolha uma opção: ";
        cin >> choice;

        switch (choice) {
            case 1:
                int qtd;
                cout << "Digite a quantidade de valores a serem inseridos: ";
                cin >> qtd;

                cout << "Digite os valores para inserir: ";
                for (int i = 0; i < qtd; i++) {
                    int v;
                    cin >> v;
                    root = insert(root, v, nodes, isMinHeap);
                }
                break;
            case 2:
                root = removeExtreme(root, isMinHeap);
                break;
            case 3:
                cout << "Percurso Nível: ";
                levelOrder(root);
                break;
            case 4:
                saveGraphToFile(root, "heap.dot");
                break;
            case 5:
                heapify(root, isMinHeap);
                cout << "Heap reestruturada com sucesso!\n";
                break;
            case 6:
                cout << "Saindo...\n";
                return 0;
            default:
                cout << "Opção inválida.\n";
        }
    }
}
