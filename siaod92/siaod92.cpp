#include <iostream>
#include <string>
#include <chrono>

using namespace std;

int N = 250000;

struct Product
{
    Product(char name[16], string articul, char cur, unsigned int price)
    {
        memcpy(this->name, name, sizeof(this->name)); // присваиваем полю name параметр name
        this->articul = articul;
        this->cur = cur;
        this->price = price;
    }
    char name[16];
    string articul;
    char cur;
    unsigned int price : 8;
};

struct HashTable
{
    int size;
    struct Node
    {
        Node(Product* ptr)
        {
            this->ptr = ptr;
            next_ptr = nullptr;
        }
        Product* ptr;
        Node* next_ptr;
    };
    Node* table[10];
    HashTable() 
    {
        size = 0;
        for (int i = 0; i < 10; i++)
        {
            table[i] = nullptr;
        }
    }
    void func(Product* ptr);
    Product* find(string id);
};

Product* HashTable::find(string id)
{
    int count = 0;
    auto begin = std::chrono::high_resolution_clock::now();
    int index = (id[id.size() - 1] - '0');
    if (this->table[index]->ptr->articul == id)
    {
        count++;
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        cout << "ms : " << elapsed_ms.count() << " counts = " << count << endl;
        return this->table[index]->ptr;
    }
    else
    {
        if (this->table[index]->next_ptr != nullptr)
        {
            count++;
            Node* temp = this->table[index]->next_ptr;
            while (temp->ptr->articul != id and temp->next_ptr != nullptr)
            {
                count++;
                temp = temp->next_ptr;
            }
            if (temp->ptr->articul == id)
            {
                count++;
                auto end = std::chrono::high_resolution_clock::now();
                auto elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
                cout << "ms : " << elapsed_ms.count() << " counts = " << count << endl;
                return temp->ptr;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        cout << "ms : " << elapsed_ms.count() << " counts = " << count << endl;
        return nullptr;
    }
}

void HashTable::func(Product* ptr)
{
    int index = (ptr->articul[ptr->articul.size()-1] - '0');
    if (table[index] != nullptr)
    {
        Node* temp = table[index];
        while (temp->next_ptr != nullptr)
        {
            temp = temp->next_ptr;
        }
        temp->next_ptr = new Node(ptr);
    }
    else
        table[index] = new Node(ptr);
    size++;
}

void FillArray(HashTable* table)
{
    string assortiment[3] =
    { "Дыня","Арбуз","Яблоко" };
    string articul = "000000";
    char* a;
    long long int i = 0;
    srand(time(0));
    while (i < N)
    {
        a = &assortiment[rand() % 3][0];
        for (int j = 0; j < to_string(i).length(); j++)
        {
            articul[articul.size() - 1 - j] = to_string(i)[to_string(i).length() - 1 - j];
        }
        char rub_dol[2] = { 'r','$' };
        table->func(new Product(a, articul, rub_dol[rand() % 2], rand() % 255));
        i++;
    }
}

void Print(HashTable* table)
{
    for (int i = 0; i < 10; i++)
    {
        cout << table->table[i]->ptr->articul << " " << table->table[i]->ptr->name << " " << table->table[i]->ptr->price << table->table[i]->ptr->cur << endl;
        HashTable::Node* ptr = table->table[i]->next_ptr;
        while (ptr != nullptr)
        {
            cout << ptr->ptr->articul << " " << ptr->ptr->name << " " << ptr->ptr->price << ptr->ptr->cur << endl;
            ptr = ptr->next_ptr;
        }
    }
}


int main()
{
    system("chcp 1251 > null");
    HashTable table;
    FillArray(&table);
    Print(&table);
    string articul;
    cout << "Articul : "; cin >> articul;
    Product* ptr = table.find(articul);
    if (ptr != nullptr)
        cout  << ptr->name << " " << ptr->price << ptr->cur;
    else
        cout << "Объекта под артиклуом " << articul << " не существует";
}
