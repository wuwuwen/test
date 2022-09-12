#include"dictionary.h"

int string_to_ascii(string& ptr)
{
    int num = 0;
    for (int i = 0; ptr[i] != 0; ++i)
        num += ptr[i] - '0';
    return num;
}


Word::Word(string str)
{
    English = str.substr(0, str.find('|'));
    Chinese = str.substr(str.find('|') + 1);
    key = string_to_ascii(English);
}
void Word::show()
{
   cout << English << "      " << Chinese << endl;
}

Dictionary::Dictionary(const char* dir) : dir(dir)
{
ifstream is(dir, ios::out);
while (!is.eof())
{
    char buf[1024] = { 0 };
    is.getline(buf, 1024);
    Word word(buf);
    first_char_map_52.insert(make_pair((int)buf[0], word));
    word_map.insert(make_pair(word.key, word));
}
    for (int i = 'a'; i <= 'z'; ++i)
        first_char_map_1.insert(make_pair(i, first_char_map_52.equal_range(i)));
    for (int i = 'A'; i <= 'P'; ++i)
        first_char_map_1.insert(make_pair(i, first_char_map_52.equal_range(i)));
}
    void Dictionary::find_word(string find_word)
{
    int flag = 0;
    multimap<int, pair<multimap<int, Word>::iterator, multimap<int, Word>::iterator>>::iterator i = first_char_map_1.find(find_word[0]);
    for (multimap<int, Word>::iterator it = i->second.first; it != i->second.second; it++)
    {
        if (it->second.English == find_word)
        {
            flag = 1;
            cout << it->second.English << endl;
            for (int i = 0; it->second.Chinese[i] != 0; ++i)
            {
                if (it->second.Chinese[i] == '|')
                {
                    it->second.Chinese[i] = '\0';
                    cout << endl;
                    i++;
                }
                cout << it->second.Chinese[i];
            }
        }
    }
    if (!flag)
        cout << "�ʵ�û�иô�: " << find_word << endl;
}
Word& Dictionary::find_word_class(string find_word)
{
    static Word return_word;
    int flag = 0;
    multimap<int, pair<multimap<int, Word>::iterator, multimap<int, Word>::iterator>>::iterator i = first_char_map_1.find(find_word[0]);
    for (multimap<int, Word>::iterator it = i->second.first; it != i->second.second; it++)
    {
        if (it->second.English == find_word)
        {
            flag = 1;
            return_word = it->second;
        }
    }
    if (!flag)
        return_word = Word(find_word.append("|�ʵ�û�иô�"));
    return return_word;
}

void Dictionary::show_first_char_map_all_or_one(int first_char)
{
    if (first_char == 0)
    {
        for (multimap<int, Word>::iterator it = first_char_map_52.begin(); it != first_char_map_52.end(); ++it)
        {
            cout << (char)it->first << "   ";
            it->second.show();
        }
        return;
    }

    multimap<int, Word>::iterator it = first_char_map_52.find(first_char);
    if (it != first_char_map_52.end())
    {
        int num = 0;
        for (; it != first_char_map_52.end() && num < first_char_map_52.count(first_char); ++it, ++num)
        {
            cout << (char)it->first << "   ";
            it->second.show();
        }
    }

    else
        cout << "\n�ʵ�û�и���ĸ��ͷ�ĵ���:" << (char)first_char << endl;
}


New_words_notebook::New_words_notebook(Dictionary& dictionary, const char* dir) : dir(dir), dictionary(dictionary)
{
    ifstream is(dir, ios::out);
    while (!is.eof())
    {
        char buf[1024] = { 0 };
        is.getline(buf, 1024);
        Word new_word(buf);
        if (new_word.Chinese != "")
            new_word_map.insert(make_pair(new_word.key, new_word));
    }
}
void New_words_notebook::add_new_word(string new_word_English)
{
    for (multimap<int, Word>::iterator it = new_word_map.begin(); it != new_word_map.end(); ++it)
    {
        if (it->second.English == new_word_English)
        {
            cout << "\n���ʱ�����Ӹô�: " << new_word_English << endl;
            return;
        }
    }
    Word add_new_word = dictionary.find_word_class(new_word_English);
    if (add_new_word.Chinese == "�ʵ�û�иô�")
    {
        cout << "�ʵ���û�иô�" << new_word_English << endl;
        return;
    }
    new_word_map.insert(make_pair(add_new_word.key, add_new_word));
    cout << add_new_word.English << " ��ӳɹ�" << endl;
}
void New_words_notebook::del_new_word(string del_word_English)
{
    int flag = 1;
    Word del_word = dictionary.find_word_class(del_word_English);
    for (multimap<int, Word>::iterator it = new_word_map.begin(); it != new_word_map.end(); ++it)
    {
        if (it->second.English == del_word.English)
        {
            flag = 0;
            new_word_map.erase(it);
            cout << del_word_English << " �����ʱ�ɾ���ɹ�" << endl;
            break;
        }
    }
    if (flag)
        cout << "���ʱ�û�иô�: " << del_word_English << endl;
}
void New_words_notebook::show()
{
    int i = 1;
    for (multimap<int, Word>::iterator it = new_word_map.begin(); it != new_word_map.end(); ++it)
    {
        cout << "����" << i << ":  ";
        i++;
        it->second.show();
    }
}
New_words_notebook::~New_words_notebook()
{
    ofstream os(dir, ios::trunc | ios::in);
    for (multimap<int, Word>::iterator it = new_word_map.begin(); it != new_word_map.end(); ++it)
        os << it->second.English.append("|").append(it->second.Chinese).append("\n");
}


My_program::My_program(const char* dictionary_dir, const char* new_words_notebook_dir) : dictionary(dictionary_dir), new_words_notebook(dictionary, new_words_notebook_dir)
{
}
void My_program::run()
{
    string find_word_buf, del_word_buf, first_word_buf, son_mode_num, main_mode_num;
    int flag = 1;
    while (flag)
    {
        cout << "\n";
        cout << "*************************************************************************" << endl;
        cout << "*  ���˵�:  1.�鵥��  2.�鿴���ʱ�  3.��ʾ����ĸΪx�����е���  0.�˳�   *" << endl;
        cout << "*************************************************************************" << endl;
        cout << "-> ";
        cin >> main_mode_num; system("cls");
        switch (main_mode_num[0] - '0')
        {
        case 1:
            while (1)
            {
                cout << "\n";
                cout << "**********************************************" << endl;
                cout << "*       �鵥��ģʽ        0.�������˵�       *" << endl;
                cout << "**********************************************" << endl;
                cout << "-> ";
                cin >> find_word_buf;

                if (find_word_buf[0] == '0')
                    break;
                dictionary.find_word(find_word_buf);
                if (dictionary.find_word_class(find_word_buf).Chinese != "�ʵ�û�иô�")
                {
                    cout << "\n \n"
                        << find_word_buf << "�Ƿ���ӵ����ʱ�[y/n]: ";
                    cin >> son_mode_num; system("cls");
                    if (son_mode_num[0] == 'y')
                        new_words_notebook.add_new_word(find_word_buf);
                }
            }
            break;
        case 2:
            while (1)
            {
                cout << "\n";
                cout << "****************************************************" << endl;
                cout << "*  ���ʱ�  1.��ʾ���ʱ�  2.ɾ������  0.�������˵�  *" << endl;
                cout << "****************************************************" << endl;
                cout << "-> ";
                cin >> son_mode_num;
                switch (son_mode_num[0] - '0')
                {
                case 1:
                    new_words_notebook.show();
                    break;
                case 2:
                    cout << "����Ҫɾ���ĵ���: ";
                    cin >> del_word_buf; system("cls");
                    new_words_notebook.del_new_word(del_word_buf);
                    break;
                case 0:
                    break;
                default:
                    cout << "û�иò���" << son_mode_num << endl;
                    break;
                }
                if (son_mode_num[0] == '0')
                    break;
            }
            break;
        case 0:
            flag = 0;
            break;
        case 3:
            while (1)
            {
                cout << "\n";
                cout << "***************************************************" << endl;
                cout << "*   ��ʾ����ĸΪx�����е���      0.�������˵�     *" << endl;
                cout << "***************************************************" << endl;
                cout << "x -> ";
                cin >> first_word_buf;
                if (first_word_buf[0] == '0')
                    break;
                dictionary.show_first_char_map_all_or_one(first_word_buf[0]);
            }
            break;
        default:
            cout << "û�иò���" << main_mode_num << endl;
            break;
        }
    }
}


