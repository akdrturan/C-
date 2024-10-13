#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
// Gerekli kütüphanelerin tanımlanması
using namespace std;

const int GRID_SIZE = 14; //oluşturulan ızgaranın boyutunu belirtir
const char EMPTY = 'e'; //boş olan (ulaşılabilir olan) karoları temsil eder
const char UNREACHABLE = 'U';// ulaşılamaz karoları temsil eder
const char START = 'S'; //başlangıç noktasını temsil eder
const char TARGET = 'T'; //hedef noktasını temsil eder
const char WIRE = 'W'; //tel içeren karoları temsil eder

struct Point {
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}  //Bu yapı, bir noktanın koordinatlarını temsil eder. x ve y veri elemanları, noktanın ızgara üzerindeki konumunu belirtir.
};

// Yön vektörleri (sağ, sol, aşağı, yukarı)
int dirX[] = {1, -1, 0, 0};
int dirY[] = {0, 0, 1, -1};

/*Bu diziler, ızgara üzerinde dört yönlü hareket için kullanılır. Her biri, sağa, sola, aşağıya ve yukarıya hareket etmek için x ve y koordinatlarındaki değişiklikleri tanımlar.
Örneğin, dirX[0] ve dirY[0] sağa doğru hareketi temsil ederken, dirX[1] ve dirY[1] sola doğru hareketi temsil eder.*/

bool isValid(int x, int y, vector<vector<char>>& grid) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE && (grid[x][y] == EMPTY || grid[x][y] == TARGET);
}
/*isValid fonksiyonu, verilen koordinatların geçerli olup olmadığını kontrol eder.*/
void printGrid(const vector<vector<char>>& grid) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            cout << grid[i][j] << '\t';
        }
        cout << endl;
    }
}
/*printGrid Fonksiyonu, ızgaranın her bir elemanını ekrana yazdırarak, ızgaranın mevcut durumunu görüntüler. 
Her bir karo, ızgara üzerindeki durumunu (EMPTY, UNREACHABLE, START, TARGET, WIRE) gösterir.*/

void generateRandomGrid(vector<vector<char>>& grid, Point& start, Point& target) {
    srand(time(0));
    //generateRandomGrid rastgele bir ızgara oluşturmak için kullanılır
    // Başlangıç ve hedef karoları yerleştir
    start = Point(rand() % GRID_SIZE, rand() % GRID_SIZE);
    target = Point(rand() % GRID_SIZE, rand() % GRID_SIZE);
    //start ve target noktaları, ızgaranın sınırları içinde rastgele bir x ve y koordinatı seçilerek belirlenir.
    while (start.x == target.x && start.y == target.y) {
        target = Point(rand() % GRID_SIZE, rand() % GRID_SIZE);
    }
    //while ile başlangıç ve hedef noktalarının farklı olmasını sağlanır.
    grid[start.x][start.y] = START;
    grid[target.x][target.y] = TARGET;
    // Belirlenen başlangıç ve hedef noktaları, ızgaradaki ilgili koordinatlara yerleştirilir.
    // Ulaşılamaz karoları yerleştir
    int unreachableCount = 20; //ulaşılamaz karolar
    while (unreachableCount > 0) {
        int x = rand() % GRID_SIZE;
        int y = rand() % GRID_SIZE;
        if (grid[x][y] == EMPTY) {
            grid[x][y] = UNREACHABLE;
            --unreachableCount;
        } /*Bu işlemler ile, ızgaradaki rastgele noktalara ulaşılamaz durumu ekler. Bu noktalar, başlangıç veya hedef noktaları olamazlar ve tel döşeme işlemi sırasında atlanır*/
    }
}
/* findShortestPath, Breadth-First Search, BFS algoritmasını kullanarak başlangıç noktasından hedef noktasına en kısa yolu bulmaya çalışır.*/
void findShortestPath(vector<vector<char>>& grid, Point start, Point target) {
    queue<Point> q;
    vector<vector<int>> distance(GRID_SIZE, vector<int>(GRID_SIZE, -1));
    vector<vector<Point>> parent(GRID_SIZE, vector<Point>(GRID_SIZE, Point(-1, -1)));

    q.push(start); //kuyruğa(queue) başlangıç noktası eklenir.
    distance[start.x][start.y] = 0; //Breadth-First Search başlangıcında, kuyruğa başlangıç noktası eklenir.

    bool found = false;
    //while ile hedef bulunmadığı sürece döngü devam eder.
    while (!q.empty() && !found) {
        Point current = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int newX = current.x + dirX[i]; //Yeni koordinatlar için
            int newY = current.y + dirY[i];

            if (isValid(newX, newY, grid) && distance[newX][newY] == -1) {
                q.push(Point(newX, newY));
                distance[newX][newY] = distance[current.x][current.y] + 1;
                parent[newX][newY] = current;
                if (newX == target.x && newY == target.y) {
                    found = true;
                    break;
                }
            }
        }
    }

    if (distance[target.x][target.y] != -1) {
        // Hedefe ulaşıldı, yol oluşturuluyor
        Point p = target;
        while (!(p.x == start.x && p.y == start.y)) {
            if (grid[p.x][p.y] != TARGET) {
                grid[p.x][p.y] = WIRE;
            }
            p = parent[p.x][p.y];
        }
    }
}
/*BFS algoritması ile bulunan en kısa yolu ızgarada tel olarak gösterir.
parent dizisi, her bir karo için bir önceki karonun kaydını tutar, bu da yolu geriye takip etmek için kullanılır. 
Bu şekilde, başlangıç noktasından hedef noktasına olan en kısa yolu ızgarada görsel olarak işaretler*/
int main() {
    vector<vector<char>> grid(GRID_SIZE, vector<char>(GRID_SIZE, EMPTY));
    Point start(0, 0), target(0, 0);

    generateRandomGrid(grid, start, target);
    findShortestPath(grid, start, target);

    printGrid(grid);
/*main ile programın temel işlevlerini başlatır, işler ve sonuçları görüntüler*/
    return 0;
}
