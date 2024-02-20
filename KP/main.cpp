#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

struct Node {
  int key, prior, size;
  Node *left, *right;
};

using namespace std;

int Size(Node* root) {
  if (!root) {
    return 0;
  } else {
    return root->size;
  }
}

pair<Node*, Node*> split(Node* root, int key) {
  if (!root) {
    return {NULL, NULL};
  } else if (key < root->key) {
    pair<Node*, Node*> q = split(root->left, key);
    Node* r = (Node*)malloc(sizeof(Node));
    r->right = root->right;
    r->key = root->key;
    r->prior = root->prior;
    r->left = q.second;
    r->size = Size(r->right) + Size(r->left) + 1;
    return {q.first, r};
  } else {
    pair<Node*, Node*> q = split(root->right, key);
    Node* l = (Node*)malloc(sizeof(Node));
    l->left = root->left;
    l->key = root->key;
    l->prior = root->prior;
    l->right = q.first;
    l->size = Size(l->left) + Size(l->right) + 1;
    return {l, q.second};
  }
}

void insert(Node* root, int key, int prior, Node** newRoot) {
  *newRoot = (Node*)malloc(sizeof(Node));

  if (!root || prior > root->prior) {
    pair<Node*, Node*> q = split(root, key);
    (*newRoot)->left = q.first;
    (*newRoot)->right = q.second;
    (*newRoot)->key = key;
    (*newRoot)->prior = prior;
    (*newRoot)->size = Size(q.first) + Size(q.second) + 1;
    return;
  }

  (*newRoot)->key = root->key;
  (*newRoot)->prior = root->prior;
  (*newRoot)->size = root->size + 1;
  if (key < root->key) {
    (*newRoot)->right = root->right;
    insert(root->left, key, prior, &((*newRoot)->left));
  } else {
    (*newRoot)->left = root->left;
    insert(root->right, key, prior, &((*newRoot)->right));
  }
}

void debug_print_helper(Node* v, int indent) {
  if (v) {
    if (indent) {
      cout << setw(indent) << " ";
    }
    cout << v->key << ":" << v->prior << ":" << v->size << "\n";
    debug_print_helper(v->left, indent + 4);
    debug_print_helper(v->right, indent + 4);
  }
}

Node* search(Node* root, int key) {
  if (!root) {
    return NULL;
  } else if (root->key == key) {
    return root;
  } else if (key < root->key) {
    return search(root->left, key);
  } else {
    return search(root->right, key);
  }
}

Node* merge(Node* l, Node* r) {
  if (!l) {
    return r;
  }
  if (!r) {
    return l;
  }

  Node* newRoot = (Node*)malloc(sizeof(Node));
  if (l->prior > r->prior) {
    newRoot->key = l->key;
    newRoot->prior = l->prior;
    newRoot->left = l->left;
    newRoot->right = merge(l->right, r);
    newRoot->size = Size(newRoot->left) + Size(newRoot->right) + 1;
  } else {
    newRoot->key = r->key;
    newRoot->prior = r->prior;
    newRoot->right = r->right;
    newRoot->left = merge(l, r->left);
    newRoot->size = Size(newRoot->left) + Size(newRoot->right) + 1;
  }

  return newRoot;
}

void del_help(Node* root, int key, Node** newRoot) {
  if (root->key == key) {
    *newRoot = merge(root->left, root->right);
    return;
  }

  *newRoot = (Node*)malloc(sizeof(Node));
  (*newRoot)->key = root->key;
  (*newRoot)->prior = root->prior;
  (*newRoot)->size = root->size - 1;
  if (key < root->key) {
    (*newRoot)->right = root->right;
    del_help(root->left, key, &((*newRoot)->left));
  } else {
    (*newRoot)->left = root->left;
    del_help(root->right, key, &((*newRoot)->right));
  }
}

void del(Node* root, int key, Node** newRoot) {
  if (!search(root, key)) {
    *newRoot = root;
    return;
  }

  del_help(root, key, newRoot);
}

struct Point {
  int x;
  int y;
  bool type;
  Node* root;
};

// int bSearch(vector<Point> a, int x) {}

bool comparePoints(Point a, Point b) {
  if (a.x != b.x) {
    return a.x < b.x;
  } else {
    return a.type > b.type;
  };
}

int request(Node* root, int y) {
  if (!root) {
    return 0;
  }

  if (root->key <= y) {
    return request(root->right, y);
  } else {
    return 1 + Size(root->right) + request(root->left, y);
  }
}

int binarySearch(vector<Point> points, int targetX) {
  int left = -1;
  int right = points.size();

  while (left + 1 < right) {
    int mid = left + (right - left) / 2;

    if (points[mid].x <= targetX) {
      left = mid;
    } else {
      right = mid;
    }
  }

  return left;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  srand(time(0));
  vector<Point> points;
  int n, m;

  cin >> n >> m;
  for (int i = 0; i < n; i++) {
    int x1, x2, y;
    cin >> x1 >> x2 >> y;
    points.push_back({x1, y, true, NULL});
    points.push_back({x2, y, false, NULL});
  }

  sort(points.begin(), points.end(),
       [](const Point a, const Point& b) { return a.x < b.x; });

  vector<Point> unique_points;
  Node *prevRoot = NULL, *newRoot = NULL;
  for (int i = 0; i < n * 2; i++) {
    if (points[i].type == true) {
      insert(prevRoot, points[i].y, rand(), &newRoot);
    } else {
      del(prevRoot, points[i].y, &newRoot);
    }
    if (i == n * 2 - 1 ||
        i != n * 2 - 1 && (points[i].x != points[i + 1].x ||
                           points[i].type != points[i + 1].type)) {
      unique_points.push_back(
          {points[i].x, points[i].y, points[i].type, newRoot});
      // cout << points[i].x << ":" << points[i].type << "\n";
    }
    points[i].root = newRoot;
    prevRoot = newRoot;
  }

  int x, y;
  for (int i = 0; i < m; i++) {
    cin >> x >> y;
    int index = binarySearch(unique_points, x);
    if (index == -1) {
      cout << 0 << "\n";
    } else {
      if (unique_points[index].x == x && unique_points[index].type == false) {
        index--;
      }
      // cout << i << "- " << points[index].x << ":" << points[index].y << ":"
      //      << points[index].type << "\n";
      cout << request(unique_points[index].root, y) << "\n";
    }
  }

  // cout << "\n\nВСЕ ТОЧКИ\n\n";
  // for (int i = 0; i < unique_points.size(); i++) {
  //   cout << "\n\n";
  //   cout << i << "- " << unique_points[i].x << ":" << unique_points[i].y <<
  //   ":"
  //        << unique_points[i].type << "\n";
  //   debug_print_helper(unique_points[i].root, 0);
  //   cout << "\n\n";
  // }
}