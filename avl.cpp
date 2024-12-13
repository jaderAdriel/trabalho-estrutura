#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

struct Node {
    int value;
    Node* left;
    Node* right;
    int height;

    Node(int val) : value(val), left(nullptr), right(nullptr), height(1) {}
};

// Função para obter a altura do nó
int getHeight(Node* node) {
    return node ? node->height : 0;
}

// Função para calcular o fator de balanceamento
int getBalanceFactor(Node* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// Rotação à direita
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T = x->right;

    // Realiza a rotação
    x->right = y;
    y->left = T;

    // Atualiza as alturas
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Rotação à esquerda
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T = y->left;

    // Realiza a rotação
    y->left = x;
    x->right = T;

    // Atualiza as alturas
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Função auxiliar para inserção
Node* insertRec(Node* node, int value) {
    if (!node) return new Node(value);

    if (value < node->value) {
        node->left = insertRec(node->left, value);
    } else if (value > node->value) {
        node->right = insertRec(node->right, value);
    } else {
        // Duplicados não são permitidos
        return node;
    }

    // Atualiza a altura do nó
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

    // Calcula o fator de balanceamento
    int balance = getBalanceFactor(node);

    // Verifica os casos de desbalanceamento
    if (balance > 1 && value < node->left->value) {
        return rotateRight(node); // Rotação simples à direita
    }
    if (balance < -1 && value > node->right->value) {
        return rotateLeft(node); // Rotação simples à esquerda
    }
    if (balance > 1 && value > node->left->value) {
        node->left = rotateLeft(node->left);
        return rotateRight(node); // Rotação dupla: esquerda-direita
    }
    if (balance < -1 && value < node->right->value) {
        node->right = rotateRight(node->right);
        return rotateLeft(node); // Rotação dupla: direita-esquerda
    }

    return node;
}

// Função para buscar um valor na árvore
Node* search(Node* root, int value) {
    if (!root || root->value == value) return root;
    if (value < root->value) return search(root->left, value);
    return search(root->right, value);
}

// Função auxiliar para encontrar o nó com o valor mínimo
Node* getMinNode(Node* node) {
    Node* current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

// Função auxiliar para remoção de um nó
Node* deleteRec(Node* root, int value) {
    if (!root) return root;

    // Realiza a busca do nó a ser removido
    if (value < root->value) {
        root->left = deleteRec(root->left, value);
    } else if (value > root->value) {
        root->right = deleteRec(root->right, value);
    } else {
        // Nó a ser removido encontrado
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            delete root;
            return temp;
        }

        // Nó com dois filhos
        Node* temp = getMinNode(root->right);
        root->value = temp->value;
        root->right = deleteRec(root->right, temp->value);
    }

    // Atualiza a altura do nó
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

    // Verifica o fator de balanceamento e faz a rotação necessária
    int balance = getBalanceFactor(root);

    if (balance > 1 && getBalanceFactor(root->left) >= 0) {
        return rotateRight(root); // Rotação simples à direita
    }
    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root); // Rotação dupla: esquerda-direita
    }
    if (balance < -1 && getBalanceFactor(root->right) <= 0) {
        return rotateLeft(root); // Rotação simples à esquerda
    }
    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root); // Rotação dupla: direita-esquerda
    }

    return root;
}

// Funções auxiliares para percursos
void preOrder(Node* node) {
    if (node) {
        cout << node->value << " ";
        preOrder(node->left);
        preOrder(node->right);
    }
}

void inOrder(Node* node) {
    if (node) {
        inOrder(node->left);
        cout << node->value << " ";
        inOrder(node->right);
    }
}

void postOrder(Node* node) {
    if (node) {
        postOrder(node->left);
        postOrder(node->right);
        cout << node->value << " ";
    }
}

// Função para gerar o gráfico em formato Graphviz (DOT)
void generateGraphviz(Node* node, ofstream& file, int& nodeId) {
    if (node == nullptr) {
        return;
    }

    // Cria um nó no formato DOT
    file << "  node" << nodeId << " [label=\"" << node->value << "\"];\n";

    // Cria uma variável para o próximo nodeId
    int nextNodeId = nodeId + 1;

    // Se houver filho à esquerda, cria a aresta e chama recursivamente
    if (node->left != nullptr) {
        file << "  node" << nodeId << " -> node" << nextNodeId << ";\n";
        generateGraphviz(node->left, file, nextNodeId);
    }

    // Se houver filho à direita, cria a aresta e chama recursivamente
    if (node->right != nullptr) {
        file << "  node" << nodeId << " -> node" << nextNodeId << ";\n";
        generateGraphviz(node->right, file, nextNodeId);
    }

    // Incrementa o nodeId após processar os filhos
    nodeId = nextNodeId;
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
    int choice, value;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Inserir valor\n";
        cout << "2. Buscar valor\n";
        cout << "3. Remover valor\n";
        cout << "4. Ver Pre-ordem\n";
        cout << "5. Ver Em ordem\n";
        cout << "6. Ver Pos-ordem\n";
        cout << "7. Gerar árvore em formato DOT\n";
        cout << "8. Sair\n";
        cout << "Escolha: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Digite um valor para inserir: ";
                cin >> value;
                root = insertRec(root, value);
                break;
            case 2:
                cout << "Digite um valor para buscar: ";
                cin >> value;
                {
                    Node* result = search(root, value);
                    if (result) {
                        cout << "Valor " << value << " encontrado na árvore.\n";
                    } else {
                        cout << "Valor " << value << " não encontrado.\n";
                    }
                }
                break;
            case 3:
                cout << "Digite um valor para remover: ";
                cin >> value;
                root = deleteRec(root, value);
                break;
            case 4:
                cout << "Pre-ordem: ";
                preOrder(root);
                cout << endl;
                break;
            case 5:
                cout << "Em ordem: ";
                inOrder(root);
                cout << endl;
                break;
            case 6:
                cout << "Pos-ordem: ";
                postOrder(root);
                cout << endl;
                break;
            case 7:
                saveGraphToFile(root, "tree.dot");  // Gera o arquivo DOT
                break;
            case 8:
                return 0;
            default:
                cout << "Opção inválida. Tente novamente.\n";
        }
    }

    return 0;
}