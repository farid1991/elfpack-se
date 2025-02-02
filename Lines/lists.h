#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED

namespace aux {

///////////////////////////////////////////////////////////
// { class list
template<typename T>
	class list {
	public:
		struct Node {
			Node *prev;
			Node *next;
			T     data;
		};

		// class iterator
		class iterator {
			Node *m_node;
		public:
			iterator(Node *node = 0): m_node(node) {}
			iterator& operator++()    { m_node = m_node->next; return *this; }
			iterator& operator--()    { m_node = m_node->prev; return *this; }
			iterator  operator++(int) { m_node = m_node->next; return m_node->prev; }
			iterator  operator--(int) { m_node = m_node->prev; return m_node->next; }
			bool      operator==(const iterator &it) const   { return m_node == it.m_node; }
			bool      operator!=(const iterator &it) const   { return m_node != it.m_node; }
			T&        operator* ()                   const   { return m_node->data; }
			T*        operator->()                   const   { return &m_node->data; }

			Node* node() const { return m_node; }
		};

		// class const_iterator
		class const_iterator {
			Node *m_node;
		public:
			const_iterator(Node *node = 0)    : m_node(node) {}
			const_iterator(const iterator &it): m_node(it.node()) {}
			const_iterator& operator++()        { m_node = m_node->next; return *this; }
			const_iterator& operator--()        { m_node = m_node->prev; return *this; }
			const_iterator  operator++(int)     { m_node = m_node->next; return m_node->prev; }
			const_iterator  operator--(int)     { m_node = m_node->prev; return m_node->next; }
			bool            operator==(const const_iterator &it) const { return m_node == it.m_node; }
			bool            operator!=(const const_iterator &it) const { return m_node != it.m_node; }
			const T&        operator* ()                         const { return m_node->data; }
			const T*        operator->()                         const { return &m_node->data; }
		};

		// constructor
		list(): m_size(0)
		{
			m_head = new Node;
			m_head->prev = m_head;
			m_head->next = m_head;
		}

		list(const list<T> &X): m_size(0)
		{
			m_head = new Node;
			m_head->prev = m_head;
			m_head->next = m_head;

			insert(begin(), X.begin(), X.end());
		}

		// destructor
		~list() { clear(); delete m_head; }

		// insert element
		void insert(iterator P, const T &X = T())
		{
			Node *node = new Node;
			node->data = X;
			node->prev = P.node()->prev;
			node->next = P.node();
			node->prev->next = node;
			P.node()->prev   = node;
			++m_size;
		}

		// insert elements
		void insert(iterator P, const_iterator F, const_iterator L)
		{
			for (; F != L; ++F)
				insert(P, *F);
		}

		// erase element
		void erase(iterator P)
		{
			P.node()->prev->next = P.node()->next;
			P.node()->next->prev = P.node()->prev;
			delete P.node(); --m_size;
		}

		// erase elements
		iterator erase(iterator F, iterator L)
		{
			while (F != L) erase(F++);
			return L;
		}

		// push/pop functions
		void push_front(const T &X) { insert(m_head->next, X); }
		void push_back(const T &X)  { insert(m_head,       X); }
		void pop_front()            {  erase(m_head->next); }
		void pop_back()             {  erase(m_head->prev); }
		void clear()                {  erase(m_head->next, m_head); }

		// size functions
		bool    empty() const { return m_head->next == m_head; }
		unsigned size() const { return m_size; }

		// iterator functions
		      iterator begin()       { return m_head->next; }
		const_iterator begin() const { return m_head->next; }
		      iterator end()         { return m_head; }
		const_iterator end() const   { return m_head; }

		// element functions
		      T& front()       { return m_head->next->data; }
		const T& front() const { return m_head->next->data; }
		      T& back()        { return m_head->prev->data; }
		const T& back() const  { return m_head->prev->data; }

		// swap lists
		void swap(list<T> &X)
		{
			Node    *h = m_head; m_head = X.m_head; X.m_head = h;
			unsigned s = m_size; m_size = X.m_size; X.m_size = s;
		}

		// remove element
		void remove(const T &X)
		{
			for (iterator F = begin(); F != end(); )
				if (*F == X)
					erase(F++);
				else ++F;
		}

		// operators
		list<T>& operator=(const list<T> &X)
		{
			      iterator F1 =   begin(), L1 =   end();
			const_iterator F2 = X.begin(), L2 = X.end();

			for (; F1 != L1 && F2 != L2; ++F1, ++F2)
				*F1 = *F2;

			erase(F1, L1);
			insert(L1, F2, L2);
			return *this;
		}

	private:
		Node    *m_head;
		unsigned m_size;
	};
// } class list
///////////////////////////////////////////////////////////

} // namespace aux

#endif // LISTS_H_INCLUDED
