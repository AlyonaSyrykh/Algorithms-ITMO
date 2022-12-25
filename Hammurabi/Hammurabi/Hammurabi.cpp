// Hammurabi.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
const int SIZE = 10;

class Statistic
{

private:
    int population;//текущее население
    double bushel;//кол-во бушелей пшеницы
    int acres;//кол-во акров
    bool isPlague;//была ли чума
    bool loser;//проверка на 45% умершего населения от голода
    int harvested_wheat;//кол-во собранных бушелей пшеницы
    double destroyed_wheat;//кол-во уничтоженных бушелей пшеницы
    double after_destroy_wheat;//кол-во бушелей после уничтожения грызунами
    int cost_acr;//стоимость одного акра
    int col_acres;//кол-во бушелей, собранных из одного акра
    double dead_population;//кол-во умерших
    double aliens;//приезжие
    double *percent_dead;//процент умерших
    double *person_acres;

public:
    Statistic()
    {
        population = 100;
        bushel = 2800;
        acres = 1000;
        destroyed_wheat = 0;
        isPlague = false;
        after_destroy_wheat = 0;
        destroyed_wheat = 0;
        harvested_wheat = 0;
        aliens = 0;
        percent_dead = new double[SIZE];
        person_acres = new double[SIZE];
        col_acres = 0;
        cost_acr = 0;
        dead_population = 0;
        loser = false;
    };

    void filein(std::fstream &fs, int i) {
        fs >> population;
        fs >> acres;
        fs >> bushel;
        fs >> cost_acr;
        for (int j = 0; j < i;j++)
        {
            fs >> percent_dead[j];
            fs >> person_acres[j];
        }
    }

    void fileout(int i) 
    {
        std::ofstream fout("hammurabi.txt");
        if (!fout.is_open()) 
        {
            std::cout << "Ошибка открытия файла!";
        }
        else
        {
            fout << i<<"\n";
            fout << population << "\n";
            fout << acres << "\n";
            fout << bushel << "\n";
            fout << cost_acr << "\n";
            for (int j = 0;j < i;j++) {
                fout << percent_dead[j] << "\n";
                fout << person_acres[j] << "\n";
            }
            fout.close(); // закрываем файл
        }
    }

    void conclusion() 
    {
        double P = 0;
        double L = 0;
        for (int i = 0; i < 10; i++)
        {
            P += percent_dead[i];
            L += person_acres[i];
        }
        P = P / 10;
        L = L / 10;
        if (P > 33 && L < 7) std::cout << "\n\t\t\t\nИз-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города\t.Теперь вывынуждены влачить жалкое существование в изгнании\n\t\t\t\n";
        else if (P > 10 && L < 9) std::cout << "\n\t\t\t\nВы правили железной рукой, подобно Нерону и Ивану Грозному.\tНарод вздохнул с облегчением, и никто больше не желает видеть вас правителем\n\t\t\t\n";
        else if (P > 3 && L < 10) std::cout << "\n\t\t\t\nВы справились вполне неплохо, у вас, конечно, есть недоброжелатели,\t но многие хотели бы увидеть вас во главе города снова\n\t\t\t\n";
        else 
        {
            std::cout << "\n\t\t\t\nФантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше\n\t\t\t\n";
        }

    }

    bool GetLoser() 
    {
        return loser;
    }

    void printResult(int i)
    {
        std::cout << "\n\tМой повелитель, соизволь поведать тебе \n\tВ году " << i << " твоего высочайшего правления\n\t";
        if (dead_population > 0) std::cout << (int)dead_population << " человек умерли с голоду;\n\t";
        if(aliens>0) std::cout << (int)aliens << " человек прибыли в наш великий город;\n\t";
        if (isPlague) std::cout << "Чума уничтожила половину населения;\n\t";
        std::cout << "Население города сейчас составляет " << population
            << " человек;\n\tМы собрали " << harvested_wheat
            << " бушелей пшеницы, по " << col_acres 
            << " бушеля с акра;\n\tКрысы истребили " << destroyed_wheat
            << " бушелей пшеницы, оставив " << bushel
            << " бушеля в амбарах;\n\tГород сейчас занимает " << acres 
            << " акров;\n\t1 акр земли стоит сейчас " << cost_acr 
            << " бушель.\n\tЧто пожелаешь, повелитель?\n\t";
    }

    int check(std::string s)
    {
        int number;
        std::cout << s;
        while (!(std::cin >> number))
        {
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            std::cout << s;
        }
        return number;
    }

    bool isCorrect(int i,int buy_acres, int sale_acres, int eat_acres, int sow_acres)
    {
        isPlague = false;
        srand(time(0));
        cost_acr = rand() % 9 + 17;
        if ( 
            buy_acres*cost_acr <= bushel &&
            sale_acres <= acres &&
            sow_acres <= acres+buy_acres &&
            sow_acres <= 10 * population &&
            buy_acres>=0 && sow_acres>=0 &&
            sale_acres>=0 && eat_acres>=0 &&
            eat_acres<=bushel
            )
        {
            bushel += cost_acr * (sale_acres - buy_acres);//кол-во бушелей после покупки и продажи акров земли
            acres += (buy_acres - sale_acres);//текущее кол-во акров
            srand(time(0));col_acres = rand() % 5 + 1;//кол-во бушелей, собранных из одного акра
            harvested_wheat = col_acres * sow_acres;//кол-во собранных бушелей пшеницы
            bushel += (col_acres-0.5) * sow_acres;//кол-во бушелей после посева и сбора урожая
            dead_population = population - eat_acres / 20;
            person_acres[i - 1] = eat_acres / population;
            if (dead_population > 0) percent_dead[i - 1] = dead_population / population * 100;
            else percent_dead[i - 1] = 0;
            if (percent_dead[i-1] >= 45)
            {
                loser = true;
                return true;
            }
            else
            {
                bushel -= eat_acres;
                double isRats = rand() % 7;//получение коэффициента вреда от грызунов
                if (isRats != 0) destroyed_wheat = isRats / 100 * bushel;
                after_destroy_wheat = bushel - destroyed_wheat;
                bushel = after_destroy_wheat;//вред от грызунов
                if (dead_population > 0) population -= dead_population;
                aliens = dead_population / 2 + (5 - col_acres) * bushel / 600 + 1;
                if (aliens < 0) aliens = 0;
                else if (aliens > 50) aliens = 50;
                population += aliens;
                if (rand() % 100 <= 15)//чума
                {
                    population /= 2;
                    isPlague = true;
                }
                printResult(i);
                return true;
            }
        }
        else 
        {
            std::cout << "\n\tНакосячили Вы, княже, не велите казнить, помиловать!";
            std::cout << "\n\tО, повелитель, пощади нас! У нас только " << population << " человек, " << bushel << " бушелей пшеницы и " << acres << " акров земли!";
            return false;
        }
    }

    ~Statistic() {
        delete[] person_acres;
        delete[] percent_dead;
    }
};

int main()
{
    int i = 0,p=0, buy_acres, sale_acres, eat_acres, sow_acres;
    bool correct = false;
    Statistic st;
    setlocale(LC_ALL, "rus");
    int continue_file=0,exit = 0;
    std::fstream fs;
    fs.open("hammurabi.txt", std::fstream::in);
    if (!fs.is_open()) {
        std::cout << "Ошибка открытия файла"<<std::endl;
    }
    else 
    {
        if(!fs.eof())
        {
            std::cout << "\t\nЖелаете ли Вы, Величайший из высочайших повелителей, продолжить игру? да - любая цифра, нет - 0\t\n" << std::endl;
            continue_file = st.check("\t\nВведите корректный ответ: ");
            if (continue_file != 0)
            {
                fs >> i;
                p = i;
                st.filein(fs,i);
            }
        }
        fs.close();
    }
    for (; i < 10; i++)
    {
        if (i != p)
        {
            std::cout << "\t\nЖелаете прервать игру? да - любая цифра, нет - 0\t\n";
            exit = st.check("\t\nВведите корректный ответ: ");
        }
        if (exit != 0)
        {
            st.fileout(i);
            return 0;
        }
        else 
        {
            do
            {
                correct = false;
                buy_acres = st.check("\n\tСколько акров земли повелеваешь купить? ");
                sale_acres = st.check("\n\tСколько акров земли повелеваешь продать? ");
                eat_acres = st.check("\n\tСколько бушелей пшеницы повелеваешь съесть? ");
                sow_acres = st.check("\n\tСколько акров земли повелеваешь засеять? ");
                correct = st.isCorrect(i + 1, buy_acres, sale_acres, eat_acres, sow_acres);
            }
            while (!correct);
            if (st.GetLoser())
            {
                std::cout << "\n\t\t\t\t\tВ Ы  П Р О И Г Р А Л И\n\t\t\t\t\t";
                break;
            }
        }
    }
    if(!st.GetLoser()) st.conclusion();
}



