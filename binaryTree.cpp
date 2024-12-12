#include <iostream>
#include <queue>
#include <string>
#include <fstream>
using namespace std;

// Estrutura de nó para a árvore binária
struct Node {
    int value;
    Node* left;
    Node* right;
};

// Função para inserir um nó na árvore binária
Node* insert(Node* node, int value) {
    if (node == nullptr) {
        return new Node{value, nullptr, nullptr};
    }

    if (node->left == nullptr) {
        node->left = new Node{value, nullptr, nullptr};
    } else if (node->right == nullptr) {
        node->right = new Node{value, nullptr, nullptr};
    } else {
        insert(node->left, value);
    }

    return node;
}

// Função para buscar um valor na árvore
bool search(Node* node, int value) {
    if (node == nullptr) {
        return false;
    }
    if (node->value == value) {
        return true;
    }
    return search(node->left, value) || search(node->right, value);
}

// Função para percorrer a árvore em pré-ordem
void preorder(Node* node) {
    if (node == nullptr) {
        return;
    }
    cout << node->value << " ";
    preorder(node->left);
    preorder(node->right);
}

// Função para percorrer a árvore em ordem
void inorder(Node* node) {
    if (node == nullptr) {
        return;
    }
    inorder(node->left);
    cout << node->value << " ";
    inorder(node->right);
}

// Função para percorrer a árvore em pós-ordem
void postorder(Node* node) {
    if (node == nullptr) {
        return;
    }
    postorder(node->left);
    postorder(node->right);
    cout << node->value << " ";
}

// Função para imprimir a árvore no nível
void levelOrder(Node* node) {
    if (node == nullptr) {
        return;
    }
    queue<Node*> q;
    q.push(node);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        cout << current->value << " ";

        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
}

// Função auxiliar para remover o menor valor de uma subárvore
Node* removeMin(Node* node, int& minValue) {
    if (node->left == nullptr) {
        minValue = node->value;
        Node* rightChild = node->right;
        delete node;
        return rightChild;
    }
    node->left = removeMin(node->left, minValue);
    return node;
}

// Função para remover um nó da árvore
Node* deleteNode(Node* node, int value) {
    if (node == nullptr) {
        return nullptr;
    }
    if (value < node->value) {
        node->left = deleteNode(node->left, value);
    } else if (value > node->value) {
        node->right = deleteNode(node->right, value);
    } else {
        if (node->left == nullptr) {
            Node* rightChild = node->right;
            delete node;
            return rightChild;
        } else if (node->right == nullptr) {
            Node* leftChild = node->left;
            delete node;
            return leftChild;
        } else {
            int minValue;
            node->right = removeMin(node->right, minValue);
            node->value = minValue;
        }
    }
    return node;
}

// Função auxiliar para imprimir a árvore de forma indentada
void printTree(Node* node, int depth = 0) {
    if (node == nullptr) {
        return;
    }
    printTree(node->right, depth + 1);
    cout << string(depth * 4, ' ') << node->value << endl;
    printTree(node->left, depth + 1);
}

void generateGraphviz(Node* node, ofstream& file, int& nodeId) {
    if (node == nullptr) {
        return;
    }

    // Cria um nó no formato DOT
    file << "  node" << nodeId << " [label=\"" << node->value << "\"];\n";

    // Salva o nodeId atual para restaurar após processar os filhos
    int currentNodeId = nodeId;

    // Se houver filho à esquerda, cria a aresta e chama recursivamente
    if (node->left != nullptr) {
        file << "  node" << currentNodeId << " -> node" << ++nodeId << ";\n";
        generateGraphviz(node->left, file, nodeId);
    }

    // Se houver filho à direita, cria a aresta e chama recursivamente
    if (node->right != nullptr) {
        file << "  node" << currentNodeId << " -> node" << ++nodeId << ";\n";
        generateGraphviz(node->right, file, nodeId);
    }

    // Restaura o nodeId para o próximo nó no mesmo nível
    nodeId = currentNodeId + 1;
}

// Função para gerar o arquivo DOT e salvar o gráfico
void saveGraphToFile(Node* root, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    // Começo do gráfico no formato DOT
    file << "digraph G {\n";
    file << "node [shape=circle];\n";  // Definindo o estilo do nó como "circle"

    int nodeId = 0;  // Inicializa o nodeId
    generateGraphviz(root, file, nodeId);  // Chama a função para gerar os nós e as arestas

    // Fim do gráfico
    file << "}\n";
    file.close();

    cout << "Árvore gerada no formato Graphviz em " << filename << endl;
}

int main() {
    Node* root = nullptr;
    int choice, key;

    while (true) {
        cout << "\n1. Inserir\n2. Remover\n3. Buscar\n4. Percurso Pré-Ordem\n5. Percurso Em Ordem\n6. Percurso Pós-Ordem\n7. Percurso Nível\n8. Imprimir Árvore\n9. Gerar Grafo\n10. Sair\nEscolha uma opção: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Digite o valor para inserir: ";
                cin >> key;
                root = insert(root, key);
                break;
            case 2:
                cout << "Digite o valor para remover: ";
                cin >> key;
                root = deleteNode(root, key);
                break;
            case 3:
                cout << "Digite o valor para buscar: ";
                cin >> key;
                if (search(root, key)) {
                    cout << "Valor encontrado.\n";
                } else {
                    cout << "Valor não encontrado.\n";
                }
                break;
            case 4:
                cout << "Percurso Pré-Ordem: ";
                preorder(root);
                cout << endl;
                break;
            case 5:
                cout << "Percurso Em Ordem: ";
                inorder(root);
                cout << endl;
                break;
            case 6:
                cout << "Percurso Pós-Ordem: ";
                postorder(root);
                cout << endl;
                break;
            case 7:
                cout << "Percurso Nível: ";
                levelOrder(root);
                cout << endl;
                break;
            case 8:
                cout << "Imprimir Árvore: \n";
                printTree(root);
                break;
            case 9:
                saveGraphToFile(root, "tree.dot");
                break;
            case 10:
                cout << "Saindo...\n";
                return 0;
            default:
                cout << "Opção inválida. Tente novamente.\n";
        }
    }
    return 0;
}
