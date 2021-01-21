#include <iostream>
#include <map>
#include <vector>
#include <cmath>

using namespace std;

template<typename T>

struct Node {
	T information;
	int height = 0;
	Node* right;
	Node* left;
};

template<typename T>

void fill_vec(Node<T>* tree, vector<vector<pair<T, char>>>& vec, int h, int h_max, char side) {
    if (empty(tree) || (h > h_max)) {
        return;
    }
    pair add = {tree->information, side};
    vec[h].push_back(add);
    fill_vec(tree->left, vec, ++h, h_max, 'L');
    fill_vec(tree->right, vec, h, h_max, 'R');
}

template<typename T>

bool empty(Node<T>* tree) {
    if (tree == nullptr) {
        return true;
    } else {
        return false;
    }
}

template<typename T>

int height(Node<T>* tree) {  // делаем функцию height, чтобы сделать проверку на пустое дерево, так как
    if (empty(tree)) {       // если мы будем просто обращаться к height в структуре, то есть шанс обращения к
        return 0;            // пустой ячейке памяти, и как следствие, крашу программы
    } else {
        return tree->height;
    }
}

template<typename T>

Node<T>* find_max(Node<T>* tree) {
    if (!empty(tree->right)) {
        return find_max(tree);
    }
    return tree;
}


template<typename T>

Node<T>* constructor(T new_info) {
	auto new_tree = new Node<T>;
	new_tree->information = new_info;
	new_tree->height = 1;
	new_tree->left = nullptr;
	new_tree->right = nullptr;
    return new_tree;
}

template<typename T>

int variance(Node<T>* tree) {
	return height(tree->left) - height(tree->right);
}

template<typename T>

Node<T>* small_left_rotate(Node<T>* tree) {
    Node<T>* NodeD = tree->right->left;
    Node<T>* NodeA = tree;
    tree = tree->right;
    tree->left = NodeA;
    NodeA->right = NodeD;
    NodeA->height = height(NodeD) + 1;
    if (height(tree->left) > height(tree->right)) {
        tree->height = height(tree->left) + 1;
    } else {
        tree->height = height(tree->right) + 1;
    }
    return tree;
}

template<typename T>

Node<T>* small_right_rotate(Node<T>* tree) {
    Node<T>* NodeD = tree->left->right;
    Node<T>* NodeA = tree;
    tree = tree->left;
    tree->right = NodeA;
    NodeA->left = NodeD;
    NodeA->height = height(NodeD) + 1;
    if (height(tree->left) > height(tree->right)) {
        tree->height = height(tree->left) + 1;
    } else {
        tree->height = height(tree->right) + 1;
    }
    return tree;
}

template<typename T>

Node<T>* big_left_rotate(Node<T>* tree) {
    Node<T>* NodeA = tree;
    Node<T>* NodeB = tree->right;
    Node<T>* NodeC = tree->right->left;
    Node<T>* NodeE = tree->right->left->left;
    Node<T>* NodeF = tree->right->left->right;
    tree = NodeC;
    tree->left = NodeA;
    NodeA->right = NodeE;
    tree->right = NodeB;
    NodeB->left = NodeF;
    ++tree->height;
    NodeA->height -= 2;
    --NodeB->height;
    return tree;
}

template<typename T>

Node<T>* big_right_rotate(Node<T>* tree) {
    Node<T>* NodeA = tree;
    Node<T>* NodeB = NodeA->left;
    Node<T>* NodeC = NodeB->right;
    Node<T>* NodeE = NodeC->left;
    Node<T>* NodeF = NodeC->right;
    tree = NodeC;
    tree->left = NodeB;
    NodeB->right = NodeE;
    tree->right = NodeA;
    NodeA->left = NodeF;
    ++tree->height;
    NodeA->height -= 2;
    --NodeB->height;
    return tree;;
}

template<typename T>

Node<T>* do_balanse(Node<T>* tree) {
    if (height(tree->left) > height(tree->right)) {
        tree->height = height(tree->left) + 1;
    } else {
        tree->height = height(tree->right) + 1;
    }
    if ((variance(tree) == -2) && ((variance(tree->right) == 0) || (variance(tree->right) == -1))) {
        tree = small_left_rotate(tree);
    } else if ((variance(tree) == 2) && ((variance(tree->left) == 0) || (variance(tree->left) == 1))) {
        tree = small_right_rotate(tree);
    } else if ((variance(tree) == -2) && (variance(tree->right) > 0)) {
        tree = big_left_rotate(tree);
    } else if ((variance(tree) == 2) && (variance(tree->left) < 0)) {
        tree = big_right_rotate(tree);
    }
    return tree;
}

template<typename T>

Node<T>* insert_node(Node<T>* tree, T information) {
    if (empty(tree)) {  //метка остановки рекурсии, а именно создание нового узла исходя из информации
        Node<T>* add_node = constructor(information);
        return add_node;
    }
    if (information < tree->information) {  //идем влево
        tree->left = insert_node(tree->left, information);
    } else {  //идем вправо
        tree->right = insert_node(tree->right, information);
    }
    return do_balanse(tree); //в конце делаем балансировку
}

template<typename T>

Node<T>* pop_max(Node<T>* tree) {
    if (tree->right == nullptr) { //если мы пришли к максимальному эл-ту
        return tree->left; //возвращаем вместо него то, что слева
    }
    tree->right = pop_max(tree->right); //бежим к максимуму
    return do_balanse(tree); //балансируем в конце
}

template<typename T>

Node<T>* delete_node(Node<T>* tree, T information) {
    if (empty(tree)) {
        return nullptr;
    }
    if (information < tree->information) {
        tree->left = delete_node(tree->left, information);
    } else if (information > tree->information) {
        tree->right = delete_node(tree->right, information);
    } else {
        Node<T>* left = tree->left;
        Node<T>* right = tree->right;
        delete tree;
        if (empty(right)) { //если наибольшее значение не в правом поддереве, то возвращаем наиб. в левом
            return left;
        }
        Node<T>* max_node = find_max(right); //иначе находим лист с максимальным значением в првом поддереве
        max_node->right = right; //справа все остается также
        max_node->left = pop_max(right);  //слева же мы просто убираем максимальный элемент
        return max_node;
    }
    return do_balanse(tree);
}

template<typename T>

int width(T elem) {
    int count = 0;
    while (elem != 0) {
        ++count;
        elem /= 10;
    }
    return count;
}

template<typename T>

void print_tree(Node<T>* tree, int count_of_spaces) {
    vector<vector<pair<T, char>>> vec_tree(tree->height);
    fill_vec(tree, vec_tree, 0, tree->height - 1, 'R');
    int spaces = pow(2, tree->height - 1) * count_of_spaces;
    for (vector<pair<T, char>> i : vec_tree) {
        char prev = 0;
        int n = 1;
        int width_elem = 0;
        int num_elem = 1;
        T last_elem;
        for (pair<T, char> j : i) {
            (num_elem == 1) ? (n = 1) : (n = 2);
            for (int i = 0; i < spaces * n - width_elem; ++i) {
                cout << ' ';
            }
            if (prev != j.second) {
                cout << j.first;
            }
            prev = j.second;
            ++num_elem;
            width_elem = width(j.first);
            last_elem = j.first;
        }
        spaces /= 2;
        cout << endl;
    }
}

int main() {
    Node<int>* tree = constructor(5);
    tree = insert_node(tree, 4);
    tree = insert_node(tree, 7);;
    tree = insert_node(tree, 6);
    tree = insert_node(tree, 8);
    tree = insert_node(tree, 9);
    tree = insert_node(tree, 10);
    tree = delete_node(tree, 10);
    tree = insert_node(tree, 11);
    print_tree(tree, 3);
	return 0;
}
