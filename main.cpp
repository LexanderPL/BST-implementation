#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;


struct node
{
	node *parent;
	node *left;
	node *right;
	int data;
};


string intToStr(int n) //int to string conversion
{
	string tmp, ret;
	if (n < 0) {
		ret = "-";
		n = -n;
	}
	do {
		tmp += n % 10 + 48;
		n -= n % 10;
	} while (n /= 10);
	for (int i = tmp.size() - 1; i >= 0; i--)
		ret += tmp[i];
	return ret;
}


node* find_value(int n, node *start) //if no node is found, NULL is returned
{
	if ((!start) || (start->data == n))
		return start;
	else
		if (n < start->data)
			return find_value(n, start->left);
		else
			return find_value(n, start->right);
}


node* min_val(node *v) //finding a node containing the lowest value in the tree
{
	if (v->left == NULL) return v;
	else min_val(v->left);
}


node* max_val(node *v) //finding a node containing the lowest value in the tree
{
	if (v->right == NULL) return v;
	else max_val(v->right);
}


node* next_node(node *v) //finding a substitute node for the node chosen to be deleted
{
	if (v->right) return min_val(v->right);
	else if (v->left) return max_val(v->left);
	else return v;
}


void add_node(int n, node*&start, node* parent)
{
	if (!start)
	{
		node *new_node = new node;
		new_node->parent = parent;
		new_node->data = n;
		new_node->right = NULL;
		new_node->left = NULL;
		start = new_node;
	}
	else if (n >= start->data) add_node(n, start->right, start);
	else add_node(n, start->left, start);

}


void delete_node(node *&v, node *root)
{
	if (!v->left && !v->right) //the case in which node chosen to be disposed has no branches
	{
		if (v->parent) //jeœli istnieje ojciec
		{
			if (v->parent->right) //usuwanie w rodzicu wskaŸnika na jego prawe dziecko
				if (v->parent->right->data == v->data) v->parent->right = NULL;
			if (v->parent->left) //usuwanie w rodzicu wskaŸnika na jego lewe dziecko
				if (v->parent->left->data == v->data) v->parent->left = NULL;
			delete(v);
		}
		else root = NULL;
		return;
	}

	else if (!v->left && v->right)  //the case in which node chosen to be disposed has has only the right branch
	{
		if (v->parent) //checking wheter v node isn't a root
		{
			if (v->parent->right) //checking wheter the v node is the right branch
				if (v->parent->right->data == v->data)
				{
					v->right->parent = v->parent;
					v->parent->right = v->right;
				}
			if (v->parent->left) //checking whether the v node is the left branch
				if (v->parent->left->data == v->data)
				{
					v->right->parent = v->parent;
					v->parent->left = v->right;
				}
		}
		else //the case, in which the v node is root
		{
			root = v->right;
			root->parent = NULL;
		}
		delete(v);
	}
	else if (v->left && !v->right) //while the v node has only left branch
	{
		if (v->parent)
		{
			if (v->parent->right)
				if (v->parent->right->data == v->data)
				{
					v->left->parent = v->parent;
					v->parent->right = v->left;
				}
			if (v->parent->left)
				if (v->parent->left->data == v->data)
				{
					v->left->parent = v->parent;
					v->parent->left = v->left;
				}
		}
		else
		{
			root = v->left;
			root->parent = NULL;
		}
		delete(v);
	}
	else //the case when both child nodes exist
	{
		node *next = next_node(v); //finding the replacement node
		v->data = next->data;
		delete_node(next, root);
	}
}


void show_tree(string sp, string sn, node * v)
{
	string s;
	string cr, cl, cp; //frames used in show_tree() function
	cr = cl = cp = "  ";
	cr[0] = 218; cr[1] = 196; //an ASCII code for the right branch
	cl[0] = 192; cl[1] = 196; //an ASCII code for the left branch
	cp[0] = 179; //an ASCII code for the branch extension "|"

	if (v)
	{
		s = sp;
		if (sn == cr) s[s.length() - 2] = ' ';
		show_tree(s + cp, cr, v->right);

		s = s.substr(0, sp.length() - 2);
		cout << s << sn << v->data << endl;

		s = sp;
		if (sn == cl) s[s.length() - 2] = ' ';
		show_tree(s + cp, cl, v->left);
	}
}


void delete_tree(node *v)
{

	if (v)
	{
		delete_tree(v->left);   //deleting branches below v on the left
		delete_tree(v->right);  //deleting branches below v on the right
		delete v;               //deleting the v node itself
	}
}


//=============================================================================================
int main()
{
	node *root;
	root = NULL;
	string info = ""; //the info line presented above the tree
	int n, input;


	do
	{
		cout << info << endl;
		show_tree("", "", root);

		cout << endl << "Menu:" << endl;
		cout << "1) add node" << endl;
		cout << "2) delete node" << endl;
		cout << "3) the highest value of the tree" << endl;
		cout << "4) the lowest value of the tree" << endl;
		cout << "0) exit" << endl;

		cin >> input;
		switch (input)
		{
		case 1:
			cout << "A value to add: ";
			cin >> n;
			if (find_value(n, root))
			{
				info = "A node containing this value already exists";
				break;
			}
			add_node(n, root, root);
			info = "";
			break;

		case 2:
			cout << "A value of a node to delete: ";
			cin >> n;
			{
				node *temp = find_value(n, root);
				if (!temp)
				{
					info = "A node containing this value doesn't exists";
					break;
				}
				if (temp) delete_node(temp, root);
				info = "";
			}
			break;

		case 3:
			info = "The highest value of the tree: " + intToStr(max_val(root)->data);
			break;
		case 4:
			info = "The lowest value of the tree: " + intToStr(min_val(root)->data);
			break;
		default:
			info = "You have chosen improper option";
		}

		system("cls");
	} while (input);

	delete_tree(root);    //disposing the tree from system memory
	return 0;
}
