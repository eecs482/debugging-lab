#include <iostream>
#include <unordered_set>
#include <cassert>
#include "thread.h" // P1 thread library

using std::cout;
using std::endl;
using std::unordered_set;
using std::runtime_error;

/*
 * RAII wrapper for a mutex. Acquires the mutex on construction, releases it on
 * destruction.
 */
class Lock {
private:
    mutex &m;

public:
    Lock(mutex &_m) : m(_m) {
        m.lock();
    }

    ~Lock() {
        m.unlock();
    }
};

struct Node;
Node *queue; // sentinel node
mutex queue_mutex; // protects the shared queue
unordered_set<Node*> nodes; // allocated nodes for use by invariant checker

/*
 * Represents a node in the queue.
 */
struct Node {
    int data;
    Node *next;

    Node(int val = 0) : data(val), next(nullptr) {
        nodes.insert(this);
    }

    ~Node() {
        nodes.erase(this);
    }
};

/*
 * Returns true if the structural invariants for the queue hold.
 *
 * REQUIRES: queue_lock is held by the calling thread.
 */
bool checkQueueInvariants() {
    unordered_set<Node*> seen; // keep track of nodes seen during traversal
    bool valid = true;
    Node *n = queue;

    cout << "Checking queue structural invariants..." << endl;

    while(n != nullptr) {
        if(nodes.find(n) == nodes.end()) {
            // node was destroyed but is still in the queue
            valid = false;
            break;
        }
        if(seen.find(n) != seen.end()) {
            // node is in the queue twice
            valid = false;
            break;
        }
        seen.insert(n);
        n = n->next;
    }

    if(nodes != seen) {
        // node was created but isn't in the queue
        valid = false;
    }

    return valid;
}

void enqueue(int val) {
    Lock lock(queue_mutex);
    assert(checkQueueInvariants()); // ensure queue is valid at start of function

    Node *place = queue;
    while(place->next) {
        place = place->next;
    }
    Node *newNode = new Node(val);
    place->next = newNode;

    assert(checkQueueInvariants()); //ensure queue is valid at end of function
}

int dequeue() {
    Lock lock(queue_mutex);
    assert(checkQueueInvariants()); // ensure queue is valid at start of function

    int result;
    if(queue->next) {
        Node *victim = queue->next;
        queue->next = victim->next;
        result = victim->data;
        delete victim;
    }
    else {
        throw runtime_error("error: cannot dequeue from empty queue");
    }

    assert(checkQueueInvariants()); //ensure queue is valid at end of function
    return result;
}

void main_thread(void *arg) {
    queue = new Node();

    enqueue(1);
    enqueue(2);
    enqueue(3);

    cout << dequeue() << endl;
    cout << dequeue() << endl;
    cout << dequeue() << endl;
}


int main() {
    cpu::boot((thread_startfunc_t)main_thread, (void*)0, 0);
}