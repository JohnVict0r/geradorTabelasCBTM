#include <iostream>
#include <cstring>
#include <fstream>
#include <locale.h>
//#include <array>

#define MAX_TXT 50
#define MAX_N   200

using namespace std;

int main()
{
    setlocale(LC_ALL, "Portuguese");

    char nomes[MAX_N][MAX_TXT];
    char arquivo[MAX_TXT];
    int n;
    int qt=0;

    struct inscritos
    {
        int id;
        string clube;
        string id_virtual;
        string atleta_nome;
        string ranking;
        string rankingP;
        string rating;
        string ratingP;
        string participaRAT;
        string status;
    };

    struct categorias
    {
        string categoria;
    };


    //string clubes[MAX_N][MAX_TXT];
    //array<int,100> clubes;
    inscritos inscrito[100];
    categorias c[100];



    ifstream ip("\inscritos.csv");
    if(ip.is_open())
    {
        while(ip.good())
        {

            inscrito[qt].id=qt;
            getline(ip,inscrito[qt].clube,';');
            //ip.getline(clubes[qt],';');
            getline(ip,inscrito[qt].id_virtual,';');
            getline(ip,inscrito[qt].atleta_nome,';');
            getline(ip,inscrito[qt].ranking,';');
            getline(ip,inscrito[qt].rankingP,';');
            getline(ip,inscrito[qt].rating,';');
            getline(ip,inscrito[qt].ratingP,';');
            getline(ip,inscrito[qt].participaRAT,';');
            getline(ip,inscrito[qt].status,'\n');

            //clubes.at(i)=clube;
            //clubes[quant_inscritos]=clube;



            if(qt==0)
            {
                qt++;
                continue;
            }


            cout<<"============================="<<endl;
            cout<<"nº inscrito: "<<inscrito[qt].id<<endl;
            cout<<"Clube: "<<inscrito[qt].clube<<endl;
            cout<<"Atleta: "<<inscrito[qt].atleta_nome<<endl;
            cout<<"Ranking: "<<inscrito[qt].ranking<<endl;
            cout<<"Rating: "<<inscrito[qt].rating<<endl;
            cout<<"pontos_RAT: "<<inscrito[qt].ratingP<<endl;
            cout<<"participa RAT: "<<inscrito[qt].participaRAT<<endl;
            cout<<"status: "<<inscrito[qt].status<<endl;
            cout<<"============================="<<endl;

            qt++;

        }
        inscrito[qt].id='\0';
        cout<<qt-1<<" inscrito(s)!";
        ip.close();




    }
    else
    {
        cout<<"ERRO!";
    }

    int cont=0;
    bool flag_RANK =false,flag_RAT=false;;
    c[cont].categoria=inscrito[1].ranking;
    //cout<<"structs:"<<sizeof(struct inscritos);

    for(int i=1; i<qt-1; i++)
    {
        flag_RANK=false;
        for(int j=0; j<=cont; j++)
        {
            if(inscrito[i].ranking==c[j].categoria)
            {
                flag_RANK=true;
            }



        }

        if(flag_RANK==false)
        {
            cont++;
            c[cont].categoria=inscrito[i].ranking;

        }

    }
    for(int i=1; i<qt-1; i++)
    {
        flag_RAT=false;
        for(int j=0; j<=cont; j++)
        {
            if(inscrito[i].rating==c[j].categoria)
            {
                flag_RAT=true;
            }

        }

        if(flag_RAT==false)
        {
            cont++;
            c[cont].categoria=inscrito[i].rating;

        }
    }







    cout<<"categorias:"<<endl;
    for(int i=0; i<=cont; i++)
    {
        cout<<c[i].categoria<<endl;
    }


    int a=0;
    for(int j=0; j<=cont; j++)
    {
        a=0;
        cout<<"==="<<c[j].categoria<<"==="<<endl;
        for(int i=1; i<=qt-1; i++)
        {
            if(inscrito[i].ranking==c[j].categoria)
            {
                a++;
                cout<<"atleta: "<<inscrito[i].clube<<" | "<<inscrito[i].atleta_nome<<" | pontos:"<<inscrito[i].ratingP<<endl;

            }
            if(inscrito[i].rating==c[j].categoria)
            {
                a++;
                cout<<"atleta: "<<inscrito[i].clube<<" | "<<inscrito[i].atleta_nome<<" | pontos:"<<inscrito[i].ratingP<<endl;

            }


        }
        cout<<a<<" atleta(s)";
        cout<<endl<<endl;
    }






    return 0;
}
