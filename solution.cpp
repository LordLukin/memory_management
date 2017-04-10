#include <iostream>
#include <memory>
using namespace std;
class EmptyListError : public runtime_error {
  static ostringstream cnvt;

public:
  EmptyListError() : runtime_error("Usage of empty list!") {}
};

class NotFoundError : public runtime_error {
  static ostringstream cnvt;

public:
  NotFoundError() : runtime_error("Node not found!") {}
};

class NullNodeError : public runtime_error {
  static ostringstream cnvt;

public:
  NullNodeError() : runtime_error("Cannot add empty Node pointer!") {}
};

template <typename T> class Node {
public:
  Node(const T v) : next(nullptr), previous(nullptr), value(v) {}
  shared_ptr<Node<T>> next;
  shared_ptr<Node<T>> previous;
  T value;
};

template <typename T> class List {
public:
  List();
  ~List() {}
  // dodaje element na koniec listy
  void add(shared_ptr<Node<T>> &&node);   // przenoszenie
  void add(shared_ptr<Node<T>> &nod);     // kopiowanie
  shared_ptr<Node<T>> get(const T value); // zwraca element o wskazanej wartości
  void addFirst(shared_ptr<Node<T>> node); // dodaje element na początek listy
  shared_ptr<Node<T>>
  getBackward(const T value); // zwraca element o wskazanej wartości od końca

private:
  shared_ptr<Node<T>> first;
  shared_ptr<Node<T>> last;
};
template <typename T> List<T>::List() : first(nullptr), last(nullptr) {}

template <typename T> void List<T>::add(shared_ptr<Node<T>> &&node) {
  if (!node) {
    throw NullNodeError();
  }
  if (!first) {
    first = node;
  } else if (!first->next) {
    first->next = node;
    last = node;
    last->previous = first;
  } else {
    node->previous = last;
    last->next = node;
    last = node;
  }
  node = nullptr;
}

template <typename T> void List<T>::add(shared_ptr<Node<T>> &nod) {
  if (!nod) {
    throw NullNodeError();
  }
  auto node = make_shared<Node<T>>(nod->value);
  if (!first) {
    first = node;
  } else if (!first->next) {
    first->next = node;
    last = node;
    last->previous = first;
  } else {
    node->previous = last;
    last->next = node;
    last = node;
  }
}

template <typename T> void List<T>::addFirst(shared_ptr<Node<T>> node) {
  if (!first) {
    first = node;
  } else {
    node->next = first;
    first = node;
  }
}
template <typename T> shared_ptr<Node<T>> List<T>::get(const T value) {
  if (!first) {
    throw EmptyListError();
    return nullptr;
  } else {
    shared_ptr<Node<T>> current = first;
    do {
      if (current->value == value) {
        cout << "Found value " << current->value << endl;
        return current;
      } else {
        cout << "Going through " << current->value << endl;
        current = current->next;
      }
    } while (current);
    throw NotFoundError();
    return nullptr;
  }
}

template <typename T> shared_ptr<Node<T>> List<T>::getBackward(const T value) {
  shared_ptr<Node<T>> current = last;
  if (!last) {
    current = first;
  }

  if (!first) {
    throw EmptyListError();
    return nullptr;
  } else {
    do {
      if (current->value == value) {
        cout << "Found value " << current->value << endl;
        return current;
      } else {
        cout << "Going through " << current->value << endl;
        current = current->previous;
      }
    } while (current);
    throw NotFoundError();
    return nullptr;
  }
}

int main() {
  unique_ptr<List<string>> lista;
  auto nodeNapis = make_shared<Node<string>>("napis");
  auto nodeHello = make_shared<Node<string>>("hello");
  cout << "Empty list exceptions:" << endl;
  cout << "------------------------------" << endl;
  try {
    auto node = lista->get("text");
  } catch (const EmptyListError &e) {
    cout << e.what() << endl;
  }
  cout << "------------------------------" << endl << endl;

  lista->add(nodeNapis);
  lista->add(move(nodeHello));

  cout << "Nullptr exceptions:" << endl;
  cout << "------------------------------" << endl;
  try {
    lista->add(move(nodeHello));
  } catch (const NullNodeError &e) {
    cout << e.what() << endl;
  }
  try {
    lista->add(nullptr);
  } catch (const NullNodeError &e) {
    cout << e.what() << endl;
  }
  cout << "------------------------------" << endl << endl;
  cout << "Not found exceptions:" << endl;
  cout << "------------------------------" << endl;
  try {
    auto node = lista->get("text");
  } catch (const NotFoundError &e) {
    cout << e.what() << endl;
  }
  try {
    auto node = lista->getBackward("text");
  } catch (const NotFoundError &e) {
    cout << e.what() << endl;
  }
  cout << "------------------------------" << endl << endl;

  return 0;
}
