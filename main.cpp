// ---------------------------------------------------------------
// SRM Institute of Science and Technology, Tiruchirappalli
// 21CS201J - Data Structures and Algorithms | CLA-2 Task 3
// ALGORITHM BATTLE - "WHICH DATA STRUCTURE WINS?"
// Application : Library Book Search (by Book ID)
// Student     : mohamed yunus ibrahim KS
// Register No : RA2511026050176
// ---------------------------------------------------------------
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Book {
    int id;
    string title, author;
};

long long cmpCnt = 0;      // global counter: key comparisons / slots examined
long long rotationCnt = 0; // AVL rotations

// ------------------------- Tree node (shared by BST and AVL) ---
struct Node {
    Book b;
    Node *l = nullptr, *r = nullptr;
    int h = 1; // used only by AVL
    Node(const Book &x) : b(x) {}
};

// ------------------------- Traversals --------------------------
void inorder(Node *n, vector<int> &out)   { if (!n) return; inorder(n->l, out); out.push_back(n->b.id); inorder(n->r, out); }
void preorder(Node *n, vector<int> &out)  { if (!n) return; out.push_back(n->b.id); preorder(n->l, out); preorder(n->r, out); }
void postorder(Node *n, vector<int> &out) { if (!n) return; postorder(n->l, out); postorder(n->r, out); out.push_back(n->b.id); }

int heightOf(Node *n) { return n ? 1 + max(heightOf(n->l), heightOf(n->r)) : 0; } // height in levels

void printVec(const string &label, const vector<int> &v) {
    cout << "  " << label << ": ";
    for (int x : v) cout << x << " ";
    cout << "\n";
}

// ------------------------- BST ---------------------------------
class BST {
public:
    Node *root = nullptr;

    bool insert(const Book &b) {
        Node **p = &root;
        while (*p) {
            cmpCnt++;
            if (b.id == (*p)->b.id) return false; // duplicate
            p = (b.id < (*p)->b.id) ? &(*p)->l : &(*p)->r;
        }
        *p = new Node(b);
        return true;
    }

    Node *search(int id) {
        Node *cur = root;
        while (cur) {
            cmpCnt++;
            if (id == cur->b.id) return cur;
            cur = (id < cur->b.id) ? cur->l : cur->r;
        }
        return nullptr;
    }

    int height() { return heightOf(root); }
};

// ------------------------- AVL ---------------------------------
class AVL {
    static int H(Node *n) { return n ? n->h : 0; }
    static void upd(Node *n) { n->h = 1 + max(H(n->l), H(n->r)); }

    static Node *rotR(Node *y) {
        Node *x = y->l;
        y->l = x->r;
        x->r = y;
        upd(y); upd(x);
        rotationCnt++;
        return x;
    }
    static Node *rotL(Node *x) {
        Node *y = x->r;
        x->r = y->l;
        y->l = x;
        upd(x); upd(y);
        rotationCnt++;
        return y;
    }

    Node *ins(Node *n, const Book &b, bool &ok) {
        if (!n) { ok = true; return new Node(b); }
        cmpCnt++;
        if (b.id == n->b.id) { ok = false; return n; }
        if (b.id < n->b.id) n->l = ins(n->l, b, ok);
        else                n->r = ins(n->r, b, ok);
        upd(n);
        int bal = H(n->l) - H(n->r);
        if (bal > 1) {                                   // Left heavy
            if (b.id > n->l->b.id) n->l = rotL(n->l);    // LR case
            return rotR(n);                              // LL case
        }
        if (bal < -1) {                                  // Right heavy
            if (b.id < n->r->b.id) n->r = rotR(n->r);    // RL case
            return rotL(n);                              // RR case
        }
        return n;
    }

public:
    Node *root = nullptr;

    bool insert(const Book &b) {
        bool ok = false;
        root = ins(root, b, ok);
        return ok;
    }

    Node *search(int id) {
        Node *cur = root;
        while (cur) {
            cmpCnt++;
            if (id == cur->b.id) return cur;
            cur = (id < cur->b.id) ? cur->l : cur->r;
        }
        return nullptr;
    }

    int height() { return heightOf(root); }
};

// ------------------------- Hash table (Linear Probing) ---------
class HashTable {
    vector<Book> t;
    vector<bool> used;
    int size;

public:
    long long collisions = 0;          // total occupied slots hit while inserting (extra probes)
    int insertsWithCollision = 0;      // number of insertions whose home slot was already occupied

    HashTable(int s) : t(s), used(s, false), size(s) {}

    int hash(int id) const { return id % size; }

    bool insert(const Book &b) {
        int home = hash(b.id);
        bool hadCollision = false;
        for (int i = 0; i < size; i++) {
            int j = (home + i) % size;
            cmpCnt++;
            if (!used[j]) {
                t[j] = b;
                used[j] = true;
                if (hadCollision) insertsWithCollision++;
                return true;
            }
            if (t[j].id == b.id) return false; // duplicate
            collisions++;                      // slot occupied by a different key
            hadCollision = true;
        }
        return false; // table full
    }

    int search(int id) {
        int home = hash(id);
        for (int i = 0; i < size; i++) {
            int j = (home + i) % size;
            cmpCnt++;
            if (!used[j]) return -1;           // empty slot -> not present
            if (t[j].id == id) return j;
        }
        return -1;
    }
};

// ------------------------- Dataset (40 unique records) ---------
vector<Book> dataset() {
    return {
        {1050, "Clean Code", "Robert C. Martin"},
        {1025, "The Pragmatic Programmer", "Hunt & Thomas"},
        {1075, "Introduction to Algorithms", "Cormen et al."},
        {1010, "Design Patterns", "Gamma et al."},
        {1040, "The C++ Programming Language", "Bjarne Stroustrup"},
        {1060, "Effective C++", "Scott Meyers"},
        {1090, "Effective Java", "Joshua Bloch"},
        {1005, "Head First Java", "Sierra & Bates"},
        {1015, "Python Crash Course", "Eric Matthes"},
        {1030, "Fluent Python", "Luciano Ramalho"},
        {1045, "Operating System Concepts", "Silberschatz"},
        {1055, "Computer Networks", "Andrew Tanenbaum"},
        {1065, "Database System Concepts", "Silberschatz"},
        {1080, "Artificial Intelligence: A Modern Approach", "Russell & Norvig"},
        {1095, "Pattern Recognition and Machine Learning", "Christopher Bishop"},
        {1002, "Deep Learning", "Goodfellow et al."},
        {1008, "Hands-On Machine Learning", "Aurelien Geron"},
        {1012, "Data Structures Using C", "Tenenbaum"},
        {1020, "Algorithms", "Robert Sedgewick"},
        {1028, "The Art of Computer Programming", "Donald Knuth"},
        {1035, "Discrete Mathematics", "Kenneth Rosen"},
        {1042, "Compilers: Principles and Techniques", "Aho et al."},
        {1048, "Computer Organization", "Hamacher"},
        {1052, "Code Complete", "Steve McConnell"},
        {1058, "Refactoring", "Martin Fowler"},
        {1062, "Cracking the Coding Interview", "Gayle McDowell"},
        {1070, "Algorithm Design", "Kleinberg & Tardos"},
        {1078, "Let Us C", "Yashavant Kanetkar"},
        {1085, "Programming in ANSI C", "E. Balagurusamy"},
        {1092, "Think Python", "Allen Downey"},
        {1098, "Structure and Interpretation of Computer Programs", "Abelson & Sussman"},
        {1001, "The Mythical Man-Month", "Fred Brooks"},
        {1004, "Concrete Mathematics", "Graham et al."},
        {1007, "Computer Architecture", "Hennessy & Patterson"},
        {1009, "Software Engineering", "Ian Sommerville"},
        {1013, "Grokking Algorithms", "Aditya Bhargava"},
        {1018, "Data Science from Scratch", "Joel Grus"},
        {1022, "Neural Networks and Deep Learning", "Michael Nielsen"},
        {1026, "Speech and Language Processing", "Jurafsky & Martin"},
        {1033, "The Linux Command Line", "William Shotts"}};
}

// ------------------------- Helpers -----------------------------
struct Stats { long long total = 0; int maxc = 0; int n = 0; };

void record(Stats &s, long long c) { s.total += c; s.maxc = max<long long>(s.maxc, c); s.n++; }
double avg(const Stats &s) { return s.n ? (double)s.total / s.n : 0.0; }

void showViva(BST &bst, AVL &avl, HashTable &ht, int key) {
    long long c;
    cout << "\n--- Lookup of key " << key << " ---\n";
    cmpCnt = 0; Node *a = bst.search(key); c = cmpCnt;
    cout << "BST     : " << (a ? "FOUND  -> " + a->b.title : string("NOT FOUND")) << "  (comparisons = " << c << ")\n";
    cmpCnt = 0; Node *b = avl.search(key); c = cmpCnt;
    cout << "AVL     : " << (b ? "FOUND  -> " + b->b.title : string("NOT FOUND")) << "  (comparisons = " << c << ")\n";
    cmpCnt = 0; int idx = ht.search(key); c = cmpCnt;
    cout << "Hashing : " << (idx >= 0 ? "FOUND at slot " + to_string(idx) : string("NOT FOUND")) << "  (home slot = " << ht.hash(key)
         << ", probes = " << c << ")\n";
}

// ------------------------- Main --------------------------------
int main() {
    const int TABLE_SIZE = 53; // prime number
    vector<Book> data = dataset();

    BST bst; AVL avl; HashTable ht(TABLE_SIZE);
    Stats insB, insA, insH;

    // ---- Build all three with the SAME dataset (same insertion order) ----
    for (auto &b : data) {
        cmpCnt = 0; bst.insert(b); record(insB, cmpCnt);
        cmpCnt = 0; avl.insert(b); record(insA, cmpCnt);
        cmpCnt = 0; ht.insert(b);  record(insH, cmpCnt);
    }
    long long buildRotations = rotationCnt;
    int bstHeightBuild = bst.height(), avlHeightBuild = avl.height();

    cout << "=========================================================\n";
    cout << " ALGORITHM BATTLE - Library Book Search (by Book ID)\n";
    cout << " Records: " << data.size() << "   Hash table size: " << TABLE_SIZE << "\n";
    cout << "=========================================================\n\n";

    // ---- Traversals ----
    cout << "[Tree traversals]\n";
    vector<int> v;
    cout << " BST:\n";
    v.clear(); inorder(bst.root, v);   printVec("Inorder  ", v);
    v.clear(); preorder(bst.root, v);  printVec("Preorder ", v);
    v.clear(); postorder(bst.root, v); printVec("Postorder", v);
    cout << " AVL:\n";
    v.clear(); inorder(avl.root, v);   printVec("Inorder  ", v);
    v.clear(); preorder(avl.root, v);  printVec("Preorder ", v);
    v.clear(); postorder(avl.root, v); printVec("Postorder", v);

    // ---- Successful search: every stored key ----
    Stats sB, sA, sH;
    for (auto &b : data) {
        cmpCnt = 0; bst.search(b.id); record(sB, cmpCnt);
        cmpCnt = 0; avl.search(b.id); record(sA, cmpCnt);
        cmpCnt = 0; ht.search(b.id);  record(sH, cmpCnt);
    }

    // ---- Unsuccessful search: keys NOT in the dataset ----
    vector<int> missing = {1000, 1003, 1006, 1011, 1017, 1038, 1049, 1064, 1088, 1099};
    Stats uB, uA, uH;
    for (int k : missing) {
        cmpCnt = 0; bst.search(k); record(uB, cmpCnt);
        cmpCnt = 0; avl.search(k); record(uA, cmpCnt);
        cmpCnt = 0; ht.search(k);  record(uH, cmpCnt);
    }

    // ---- Extra insertions after the structures are built ----
    vector<int> newKeys = {1031, 1047, 1068, 1083, 1096};
    Stats nB, nA, nH;
    for (int k : newKeys) {
        Book nb{k, "New Book " + to_string(k), "Unknown"};
        cmpCnt = 0; bst.insert(nb); record(nB, cmpCnt);
        cmpCnt = 0; avl.insert(nb); record(nA, cmpCnt);
        cmpCnt = 0; ht.insert(nb);  record(nH, cmpCnt);
    }

    // ---- Results ----
    cout << fixed << setprecision(2);
    cout << "\n[Part C - Observations]\n";
    cout << left << setw(44) << "Experiment" << setw(10) << "BST" << setw(10) << "AVL" << setw(10) << "Hashing" << "\n";
    cout << string(74, '-') << "\n";
    cout << setw(44) << "Successful search - avg comparisons" << setw(10) << avg(sB) << setw(10) << avg(sA) << setw(10) << avg(sH) << "\n";
    cout << setw(44) << "Successful search - max comparisons" << setw(10) << sB.maxc << setw(10) << sA.maxc << setw(10) << sH.maxc << "\n";
    cout << setw(44) << "Unsuccessful search - avg comparisons" << setw(10) << avg(uB) << setw(10) << avg(uA) << setw(10) << avg(uH) << "\n";
    cout << setw(44) << "Unsuccessful search - max comparisons" << setw(10) << uB.maxc << setw(10) << uA.maxc << setw(10) << uH.maxc << "\n";
    cout << setw(44) << "Insertion (initial 40) - avg comparisons" << setw(10) << avg(insB) << setw(10) << avg(insA) << setw(10) << avg(insH) << "\n";
    cout << setw(44) << "Insertion (5 new keys) - avg comparisons" << setw(10) << avg(nB) << setw(10) << avg(nA) << setw(10) << avg(nH) << "\n";
    cout << setw(44) << "Tree height after initial 40 (levels)" << setw(10) << bstHeightBuild << setw(10) << avlHeightBuild << setw(10) << "-" << "\n";
    cout << setw(44) << "Tree height after 5 more inserts (levels)" << setw(10) << bst.height() << setw(10) << avl.height() << setw(10) << "-" << "\n";
    cout << setw(44) << "AVL rotations (build / total)" << setw(10) << "-" << setw(10) << (to_string(buildRotations) + "/" + to_string(rotationCnt)) << setw(10) << "-" << "\n";
    cout << setw(44) << "Collisions (extra probes, total)" << setw(10) << "-" << setw(10) << "-" << setw(10) << ht.collisions << "\n";
    cout << setw(44) << "Insertions that collided" << setw(10) << "-" << setw(10) << "-" << setw(10) << ht.insertsWithCollision << "\n";

    cout << "\nTheoretical reference: log2(40) ~ 5.3 levels for a perfectly balanced tree.\n";
    cout << "Hash load factor after all inserts = " << (double)(data.size() + newKeys.size()) / TABLE_SIZE << "\n";

    // ---- Viva / technical defense mode ----
    cout << "\n[Technical Defense Mode] Enter a Book ID to search (0 to exit): ";
    int key;
    while (cin >> key && key != 0) {
        showViva(bst, avl, ht, key);
        cout << "\nEnter another Book ID (0 to exit): ";
    }
    cout << "\nDone.\n";
    return 0;
}
