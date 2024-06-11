#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class PriorityQueueArray {
private:
    vector<pair<int, pair<int, int>>> heap;

    void heapifyUp(int i) {
        while (i != 0 && heap[(i - 1) / 2] > heap[i]) {
            swap(heap[i], heap[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    void heapifyDown(int i) {
        int size = heap.size();
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int smallest = i;

        if (l < size && heap[l] < heap[i])
            smallest = l;
        if (r < size && heap[r] < heap[smallest])
            smallest = r;

        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    void push(pair<int, pair<int, int>> newPair) {
        heap.push_back(newPair);
        heapifyUp(heap.size() - 1);
    }

    void pop() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
    }

    pair<int, pair<int, int>> top() {
        if (heap.empty()) throw out_of_range("Очередь с приоритетом пуста");
        return heap.front();
    }

    bool empty() {
        return heap.empty();
    }

    int size() {
        return heap.size();
    }
};

class PriorityQueueTree {
private:
    struct Node {
        pair<int, pair<int, int>> value;
        Node* left;
        Node* right;

        Node(pair<int, pair<int, int>> val) : value(val), left(nullptr), right(nullptr) {}
    };

    Node* root;
    int count;

    Node* insert(Node* node, pair<int, pair<int, int>> value) {
        if (!node) return new Node(value);
        if (value < node->value) {
            node->left = insert(node->left, value);
        }
        else {
            node->right = insert(node->right, value);
        }
        return node;
    }

    Node* findMin(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    Node* removeMin(Node* node) {
        if (!node->left) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        node->left = removeMin(node->left);
        return node;
    }

    Node* remove(Node* node, pair<int, pair<int, int>> value) {
        if (!node) return nullptr;
        if (value < node->value) {
            node->left = remove(node->left, value);
        }
        else if (value > node->value) {
            node->right = remove(node->right, value);
        }
        else {
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            else {
                Node* minNode = findMin(node->right);
                node->value = minNode->value;
                node->right = remove(node->right, minNode->value);
            }
        }
        return node;
    }

public:
    PriorityQueueTree() : root(nullptr), count(0) {}

    void push(pair<int, pair<int, int>> value) {
        root = insert(root, value);
        count++;
    }

    pair<int, pair<int, int>> top() {
        Node* minNode = findMin(root);
        if (!minNode) throw out_of_range("Очередь с приоритетом пуста");
        return minNode->value;
    }

    void pop() {
        if (!root) throw out_of_range("Очередь с приоритетом пуста");
        root = removeMin(root);
        count--;
    }

    bool empty() const {
        return !root;
    }

    int size() const {
        return count; 
    }
};

static void printLine() {
    for (int i = 0; i < 100; i++)
    {
        cout << '-';
    }
    cout << endl;
}

static int randNumber(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

static void fillMatrix(vector<vector<int>>& matrix, int min = 0, int max = 0) {

    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            matrix[i][j] = randNumber(min, max);
        }
    }
}

static void pourWater(vector<vector<int>>& matrix, int i0, int j0, int V) {
    matrix[i0][j0] += V;
}

template <typename T>
int trapWater(vector<vector<int>>& matrix, T pq, int i0 = -1, int j0 = -1, int V = 0){
    if (i0 >= 0 && j0 >= 0) {
        pourWater(matrix, i0, j0, V);
    }

    int n = matrix.size();
    int m = matrix[0].size();

    vector<vector<bool>>visited(n, vector<bool>(m, false));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (i == 0 || j == 0 || i == n - 1 || j == m - 1)
            {
                pq.push({ matrix[i][j],{i,j} });
                visited[i][j] = true;
            }
        }
    }
    vector<pair<int, int>> directions = { {-1,0},{1,0},{0,-1},{0,1} };

    int volume = 0;
    while (pq.size())
    {
        auto temp = pq.top();
        pq.pop();
        int val = temp.first;
        int x = temp.second.first;
        int y = temp.second.second;
        for (auto it : directions)
        {
            int nx = x + it.first;
            int ny = y + it.second;
            if (nx >= 0 && ny >= 0 && nx < n && ny < m && visited[nx][ny] == false)
            {
                visited[nx][ny] = true;
                volume += max(0, val - matrix[nx][ny]);
                pq.push({ max(val,matrix[nx][ny]),{nx,ny} });
            }
        }
    }
    return volume;
}

int main() {

    setlocale(LC_ALL, "RU");
    srand(time(NULL));


    unsigned int N = 1500, M = 1500;
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> PQSTL;
    PriorityQueueArray PQArray;
    PriorityQueueTree PQTree;
    vector<vector<int>> matrix(N, vector<int>(M));
    unsigned long long volume1, volume2, volume3;
    int i0 = 10;
    int j0 = 10;
    int V = 5;
    clock_t start, end;

    fillMatrix(matrix, 0, 10);

    cout << "Выполнил: Фомин Д.А 090301-ПОВа-о23" << endl;
    printLine();

    cout << "Размер матрицы высот: " << N << "x" << M << endl;
    printLine();

    start = clock();
    volume1 = trapWater(matrix, PQSTL);
    end = clock();

    double timeResult1 = double(end - start) / CLOCKS_PER_SEC;

    cout << "Время выполнения алгоритма используя реализацию очереди с приоритетом из STL: " << timeResult1 << " секунд" << endl;
    cout << "Объём невытекшей воды равен: " << volume1 << endl;
    printLine();

    start = clock();
    volume2 = trapWater(matrix, PQArray);
    end = clock();

    double timeResult2 = double(end - start) / CLOCKS_PER_SEC;
    cout << "Время выполнения алгоритма используя реализацию очереди с приоритетом на базе массива: " << timeResult2 << " секунд" << endl;
    cout << "Объём невытекшей воды равен: " << volume2 << endl;
    printLine();

    start = clock();
    volume3 = trapWater(matrix, PQTree);
    end = clock();

    double timeResult3 = double(end - start) / CLOCKS_PER_SEC;
    cout << "Время выполнения алгоритма используя реализацию очереди с приоритетом на базе бинарного дерева: " << timeResult3 << " секунд" << endl;
    cout << "Объём невытекшей воды равен: " << volume3 << endl;
    printLine();

    cout << "Сравнение времени выполнения собственной реализации с STL:" << endl;
    cout << "Очередь с приоритетом на базе массива: " << (timeResult1 / timeResult2 * 100) << "% от реализации в STL" << endl;
    cout << "Очередь с приоритетом на базе бинарного дерева: " << (timeResult1 / timeResult3 * 100) << "% от реализации в STL" << endl;
    printLine();


    return 0;
}
