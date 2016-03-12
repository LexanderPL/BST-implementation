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


string cr, cl, cp; //ramki do wyœwietlania drzewa
node *root;

string intToStr(int n) //zamiana inta na string
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


node* find_value(int n, node *start) //funkcja zwraca wartoœæ NULL, jeœli nie znaleziono wêz³a
{
	if ((!start) || (start->data == n))
		return start;
	else
		if (n < start->data)
			return find_value(n, start->left);
		else
			return find_value(n, start->right);
}


node* min_val(node *v) //znajdowanie najmniejszej wartoœci drzewa
{
	if (v->left == NULL) return v;
	else min_val(v->left);
}


node* max_val(node *v) //znajdowanie najwiêkszej wartoœci drzewa
{
	if (v->right == NULL) return v;
	else max_val(v->right);
}


node* next_node(node *v) //znajdowanie nastêpnika
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


void delete_node(node *&v) 
{
	if (!v->left && !v->right) //przypadek, gdy usuwany wêze³ nie ma dzieci
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

	else if (!v->left && v->right)  //jeœli tylko prawy syn istnieje
	{
		if (v->parent) //sprawdzamy, czy v nie jest korzeniem
		{
			if (v->parent->right) //sprawdzamy, czy v jest prawym wêz³em
				if (v->parent->right->data == v->data)
				{
					v->right->parent = v->parent;
					v->parent->right = v->right;
				}
			if (v->parent->left) //sprawdzamy, czy v jest lewym wêz³em
				if (v->parent->left->data == v->data)
				{
					v->right->parent = v->parent;
					v->parent->left = v->right;
				}
		}
		else //gdy v jest korzeniem
		{
			root = v->right;
			root->parent = NULL;
		}
		delete(v);
	}
	else if (v->left && !v->right) //jeœli tylko lewy syn istnieje
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
	else //jeœli istniej¹ obaj synowie
	{
		node *next = next_node(v); //szukanie nastêpnika
		v->data = next->data;
		delete_node(next);
	}
}


void show_tree(string sp, string sn, node * v)
{
	string s;
	cr = cl = cp = "  ";
	cr[0] = 218; cr[1] = 196; //symbol prawego odga³êzienia
	cl[0] = 192; cl[1] = 196; //symbol lewego odga³êzienia
	cp[0] = 179; //symbol przed³u¿enia poziomu, "|"

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
		delete_tree(v->left);   // usuwamy lewe poddrzewo
		delete_tree(v->right);  // usuwamy prawe poddrzewo
		delete v;              // usuwamy sam wêze³
	}
}


//=============================================================================================
int main()
{
	root = NULL;
	string info=""; //informacja wyswietlana nad drzewem
	int n, input;
	

	do
	{
		cout << info << endl;
		show_tree("", "", root);

		cout << endl << "Menu:" << endl;
		cout << "1) dodawanie wezla" << endl;
		cout << "2) usuwanie wezla" << endl;
		cout << "3) najwieksza wartosc drzewa" << endl;
		cout << "4) najmniejsza wartosc drzewa" << endl;
		cout << "0) wyjscie z programu" << endl;

		cin >> input;
		switch (input)
		{
		case 1:
			cout << "Podaj wartosc do dodania: ";
			cin >> n;
			if (find_value(n, root))
			{
				info = "Wezel o podanej wartosci juz istnieje";
				break;
			}
			add_node(n, root, root);
			info = "";
			break;

		case 2:
			cout << "Podaj wartosc usuwanego wezla: ";
			cin >> n;
			{
				node *temp = find_value(n, root);
				if (!temp)
				{
					info = "Wezel o podanej wartosci nie istnieje";
					break;
				}
				if (temp) delete_node(temp);
				info = "";
			}
			break;

		case 3:
			info = "Najwieksza wartosc drzewa: " + intToStr(max_val(root)->data);
			break;
		case 4:
			info = "Najmniejsza wartosc drzewa: " + intToStr(min_val(root)->data);
			break;
		default:
			info = "Wybrano niewlasciwa opcje";
		}

		system("cls");
	} while (input);

	delete_tree(root);    // usuwamy drzewo z pamiêci
	return 0;
}