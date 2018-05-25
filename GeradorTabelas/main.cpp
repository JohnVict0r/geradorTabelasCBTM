#include <iostream>
#include <cstring>
#include <fstream>
#include <locale.h>
//#include <array>

#define MAX_TXT 50
#define MAX_N   200

using namespace std;

struct Inscritos
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

struct Categorias
{
    string categoria;
    int quantidade_inscritos;
};


void detalharInscrito(Inscritos i);
int contarCategorias(Inscritos inscrito[], Categorias c[],int n);

int main()
{
    setlocale(LC_ALL, "Portuguese");

    char nomes[MAX_N][MAX_TXT];
    char arquivo[MAX_TXT];
    int n;
    int qt=0;




    //string clubes[MAX_N][MAX_TXT];
    //array<int,100> clubes;
    Inscritos inscrito[100];
    Categorias c[100];



    ifstream ip("\inscritos2.csv");
    if(ip.is_open())
    {
        while(ip.good())
        {
            if(qt==0)
            {
                qt++;
                continue;
            }

            Inscritos t;

            t.id=qt;
            getline(ip,t.clube,';');
            getline(ip,t.id_virtual,';');
            getline(ip,t.atleta_nome,';');
            getline(ip,t.ranking,';');
            getline(ip,t.rankingP,';');
            getline(ip,t.rating,';');
            getline(ip,t.ratingP,';');
            getline(ip,t.participaRAT,';');
            getline(ip,t.status,'\n');
            if(t.ratingP=="" || t.status!="Inscrito")
            {
                //cout<<"!!!TECNICO!!!";
                //não pegar quando for técnico e tecnico não possuem pontos de rating no momento;
                //não pegar os pre-inscritos;
            }
            else
            {

                inscrito[qt]=t;

                //detalharInscrito(inscrito[qt]);

                qt++;

            }

            //inscrito[qt].id=qt;
            //getline(ip,inscrito[qt].clube,';');
            //ip.getline(clubes[qt],';');
            //getline(ip,inscrito[qt].id_virtual,';');
            //getline(ip,inscrito[qt].atleta_nome,';');
            //getline(ip,inscrito[qt].ranking,';');
            //getline(ip,inscrito[qt].rankingP,';');
            //getline(ip,inscrito[qt].rating,';');
            //getline(ip,inscrito[qt].ratingP,';');
            //getline(ip,inscrito[qt].participaRAT,';');
            //getline(ip,inscrito[qt].status,'\n');

            //clubes.at(i)=clube;
            //clubes[quant_inscritos]=clube;










        }

        inscrito[qt].id='\0';
        ip.close();




    }
    else
    {
        cout<<"ERRO!";
    }

    int cont=contarCategorias(inscrito,c,qt-1);



    cout<<"---RELATÓRIO DE INSCRITOS---"<<endl;

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
            if(inscrito[i].rating==c[j].categoria && inscrito[i].participaRAT=="SIM")
            {
                a++;
                cout<<"atleta: "<<inscrito[i].clube<<" | "<<inscrito[i].atleta_nome<<" | pontos:"<<inscrito[i].ratingP<<endl;

            }


        }
        c[j].quantidade_inscritos=a;
        cout<<c[j].quantidade_inscritos<<" atleta(s)";
        cout<<endl<<endl;
    }






    return 0;
}

void detalharInscrito(Inscritos i)
{

    cout<<"============================="<<endl;
    cout<<"nº inscrito: "<<i.id<<endl;
    cout<<"Clube: "<<i.clube<<endl;
    cout<<"Atleta: "<<i.atleta_nome<<endl;
    cout<<"Ranking: "<<i.ranking<<endl;
    cout<<"Rating: "<<i.rating<<endl;
    cout<<"pontos_RAT: "<<i.ratingP<<endl;
    cout<<"participa RAT: "<<i.participaRAT<<endl;
    cout<<"status: "<<i.status<<endl;
    cout<<"============================="<<endl;



}

int contarCategorias(Inscritos inscrito[], Categorias c[],int n)
{
    int cont=0;
    bool flag_RANK =false,flag_RAT=false;;
    c[cont].categoria=inscrito[1].ranking;
    //cout<<"structs:"<<sizeof(struct inscritos);

    for(int i=1; i<n; i++)
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
    for(int i=1; i<n; i++)
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

    //cout<<"categorias:"<<endl;
    //for(int i=0; i<=cont; i++)
    //{
    //    cout<<c[i].categoria<<endl;
    // }
    return cont;

}

