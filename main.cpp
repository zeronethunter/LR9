#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

template<typename T>

struct Node {
    T information = 0;
    int height = 0;
    Node* right = nullptr;
    Node* left = nullptr;
};

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

Node<T>* find_max(Node<T>* tree) {  //находим максимальный элемент, пригодится в удалении
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

void fix_height(Node<T>* tree) {  //фиксим высоту после вращений
    if (height(tree->left) > height(tree->right)) {
        tree->height = height(tree->left) + 1;
    } else {
        tree->height = height(tree->right) + 1;
    }
}

template<typename T>

int variance(Node<T>* tree) {  //разница высот, чтобы определить, нужно ли балансировать
    return height(tree->left) - height(tree->right);
}

template<typename T>

Node<T>* left_rotate(Node<T>* tree) {  //левое малое вращение
    Node<T>* change = tree->right;
    tree->right = change->left;
    change->left = tree;
    fix_height(tree);
    fix_height(change);
    return change;
}

template<typename T>

Node<T>* right_rotate(Node<T>* tree) {  //правое малое вращение
    Node<T>* change = tree->left;
    tree->left = change->right;
    change->right = tree;
    fix_height(tree);
    fix_height(change);
    return change;
}

template<typename T>

Node<T>* do_balanse(Node<T>* tree) {
    fix_height(tree);
    if (variance(tree) >= 2) {             //если различие в высоте 2, то есть если нужна балансировка
        if (variance(tree->left) < 0) {    //проверка на большое правое вращение, а именно лево-правое малые вращения
            tree->left = left_rotate(tree->left);
        }
        return right_rotate(tree);
    } else if (variance(tree) <= -2) {     //если различие -2, то есть высота правого поддерева больше высоты левого
        if (variance(tree->right) > 0) {
            tree->right = right_rotate(tree->right);
        }
        return left_rotate(tree);
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

bool flag = true; //объявляем глобальную переменную

template<typename T>

void true_print(Node<T>* tree, int spaces, int h, int need_height) {
    if (h == need_height) {  //если высота, которая нужна
            if (flag) {  //первый элемент на высоте выводим с кол-вом пробелом spaces + 1(получено экспериментальным путем:D)
                cout << setw(spaces + 1);
                if (!empty(tree)) {             //проверки на пустоту нужны, чтобы не обращаться к tree->information
                    cout << tree->information;  //так как в случае пустой ячейки будут ошибки
                } else {
                    cout << ' ';
                }
            } else {  //поледующие spaces * 2
                cout << setw(spaces * 2);
                if (!empty(tree)) {
                    cout << tree->information;
                } else {
                    cout << ' ';
                }
            }
        flag = false;
        return;  //если на ходим элемент, то дальше можно не идти
    }
    if (empty(tree)) {
        return;
    }
    true_print(tree->left, spaces, ++h, need_height);  //рекурсивно ищем элементы на нужной высоте
    true_print(tree->right, spaces, h, need_height);
}

template<typename T>

void print_tree(Node<T>* tree, int count_of_spaces) {
    int spaces = pow(2, tree->height - 1) * count_of_spaces;  //математически вычисляем правильное кол-во пробелов
    for (int i = 1; i <= tree->height; ++i) {
        flag = true;
        true_print(tree, spaces, 1, i);
        cout << endl;
        spaces /= 2;
    }
}

int main() {
    Node<int>* tree = constructor(0);
    for (int i = 1; i < 25; ++i) {  //пихаем в дерево значения от 0 до 25
        tree = insert_node(tree, i);
    }
    print_tree(tree, 3);  //выводим с разницей у листьев 3
    return 0;
}