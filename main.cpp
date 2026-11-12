// ---------------------------------------------------------------
// SRM Institute of Science and Technology, Tiruchirappalli
// 21CS201J - Data Structures and Algorithms | CLA-2 Task 3
// ALGORITHM BATTLE - "WHICH DATA STRUCTURE WINS?"
// Application : Student Record Search (by Student ID)
// Student     : Mohamed yunus ibrahim
// Register No : RA2511026050176
// ---------------------------------------------------------------
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Book {          // one student record: id, name (title), department (author)
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
        {3010, "Arun Kumar", "CSE-AIML"},
        {3015, "Priya Sharma", "CSE"},
        {3021, "Karthik Raja", "IT"},
        {3027, "Divya Lakshmi", "ECE"},
        {3034, "Mohan Das", "EEE"},
        {3040, "Sneha Reddy", "MECH"},
        {3046, "Vignesh S", "CSE-AIML"},
        {3053, "Anitha M", "CSE"},
        {3059, "Rahul Verma", "IT"},
        {3065, "Meena Devi", "ECE"},
        {3072, "Surya Prakash", "CSE-AIML"},
        {3008, "Harini K", "CSE"},
        {3081, "Dinesh Babu", "IT"},
        {3003, "Pooja Nair", "ECE"},
        {3090, "Ganesh R", "EEE"},
        {3096, "Nandhini P", "MECH"},
        {3050, "Sanjay Kumar", "CSE-AIML"},
        {3030, "Revathi S", "CSE"},
        {3018, "Ajith Kumar", "IT"},
        {3043, "Kavya Menon", "ECE"},
        {3062, "Naveen Raj", "CSE-AIML"},
        {3078, "Lavanya T", "CSE"},
        {3087, "Praveen K", "IT"},
        {3093, "Swathi R", "ECE"},
        {3006, "Bala Murugan", "EEE"},
        {3024, "Ishwarya V", "MECH"},
        {3037, "Siva Kumar", "CSE-AIML"},
        {3056, "Tamilselvi A", "CSE"},
        {3069, "Vikram Singh", "IT"},
        {3084, "Deepa Rani", "ECE"},
        {3012, "Manoj Kumar", "CSE-AIML"},
        {3099, "Janani S", "CSE"},
        {3001, "Ashwin R", "IT"},
        {3048, "Keerthana B", "ECE"},
        {3075, "Hari Prasad", "EEE"},
        {3033, "Yamini D", "MECH"},
        {3058, "Gokul Raj", "CSE-AIML"},
        {3091, "Shalini M", "CSE"},
        {3020, "Mukesh K", "IT"},
        {3067, "Bhuvana S", "ECE"}};
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
    const int TABLE_SIZE = 61; // prime number
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
    cout << " ALGORITHM BATTLE - Student Record Search (by Student ID)\n";
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
    vector<int> missing = {3000, 3002, 3005, 3009, 3014, 3028, 3045, 3060, 3079, 3098};
    Stats uB, uA, uH;
    for (int k : missing) {
        cmpCnt = 0; bst.search(k); record(uB, cmpCnt);
        cmpCnt = 0; avl.search(k); record(uA, cmpCnt);
        cmpCnt = 0; ht.search(k);  record(uH, cmpCnt);
    }

    // ---- Extra insertions after the structures are built ----
    vector<int> newKeys = {3004, 3016, 3052, 3070, 3095};
    Stats nB, nA, nH;
    for (int k : newKeys) {
        Book nb{k, "New Student " + to_string(k), "Not assigned"};
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
    cout << "\n[Technical Defense Mode] Enter a Student ID to search (0 to exit): ";
    int key;
    while (cin >> key && key != 0) {
        showViva(bst, avl, ht, key);
        cout << "\nEnter another Student ID (0 to exit): ";
    }
    cout << "\nDone.\n";
    return 0;
}
