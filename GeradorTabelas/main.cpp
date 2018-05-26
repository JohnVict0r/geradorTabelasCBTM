/*
* Autor: John Victor ALves Luiz
* Desenvolvido em: Maio de 2018
* Função: Gerar grupos aleatórios para a competições da Federação potiguar de
*         tênis de mesa. Utilizando o .CSV da relação de inscritos do site da
*         cbtm.org.br
*/

#include <iostream>
#include <cstring>
#include <fstream>
#include <locale.h>
//#include <array>



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

//categorias gerais da CBTM
struct Categorias
{
    string categoria;
    int quantidade_inscritos;
};

// É necessario normalizar as categorias da CBTM para a categorias disponíveis do evento da FPOTM
struct CategoriasFpotm
{
    string categoria;
    Categorias categoria_cbtm[10];
    int quantidade_inscritos;
    int quantidade_categorias;

};

//preparando para criar os grupos por categoria
struct AtletasCategorias
{
    CategoriasFpotm categoria_fpotm;
    Inscritos inscrito[100];

};

#define MAX_TXT 50
#define MAX_N   200

//RATINGS A à G
#define QUANTIDADE_RATINGS_MASCULINO_FPOTM 7
//RATINGS A à D
#define QUANTIDADE_RATINGS_FEMININO_FPOTM 4




void detalharInscrito(Inscritos i);
int contarCategorias(Inscritos inscrito[], Categorias c[],int n);
void relatorioInscritos(Inscritos inscrito[], Categorias c[], int n, int qt);
void criarCategoriaRatingFpotm(CategoriasFpotm categoriaInterestadual[], Inscritos inscrito[], Categorias c[],int n,int cont);
void listarCategoriasFpotm(CategoriasFpotm c[]);

int main()
{
    setlocale(LC_ALL, "Portuguese");

    char nomes[MAX_N][MAX_TXT];
    char arquivo[MAX_TXT];
    int n;
    int qt=0;

    Inscritos inscrito[100];
    Categorias c[100];
    CategoriasFpotm categoriaInterstadual[100];



    ifstream ip("\inscritos.csv");
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

    relatorioInscritos(inscrito,c,cont,qt-1);


    criarCategoriaRatingFpotm(categoriaInterstadual,inscrito,c,cont,qt-1);

    cout<<"---RELATÓRIO DE INSCRITOS FPOTM---"<<endl;
    listarCategoriasFpotm(categoriaInterstadual);

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


void relatorioInscritos(Inscritos inscrito[], Categorias c[], int n, int qt)
{
    /*
    * emite o relatório dos inscritos por categoria
    */

    int a=0;
    for(int j=0; j<=n; j++)
    {
        a=0;
        cout<<"==="<<c[j].categoria<<"==="<<endl;
        for(int i=1; i<=qt; i++)
        {
            if(inscrito[i].ranking==c[j].categoria)
            {
                a++;
                cout<<"atleta: "<<inscrito[i].clube<<" | "<<inscrito[i].atleta_nome<<" | pontos:"<<inscrito[i].ratingP<<endl;

            }
            if(inscrito[i].rating==c[j].categoria && inscrito[i].participaRAT=="SIM")
            {
                a++;
                cout<<"atleta: "<<inscrito[i].clube<<" | "<<inscrito[i].atleta_nome<<" | pontos:"<<inscrito[i].ratingP <<" | Ranking:"<<inscrito[i].ranking<<endl;

            }


        }
        c[j].quantidade_inscritos=a;
        cout<<c[j].quantidade_inscritos<<" atleta(s)";
        cout<<endl<<endl;
    }
}


void criarCategoriaRatingFpotm(CategoriasFpotm categoriaInterestadual[], Inscritos inscrito[], Categorias c[],int n,int cont)
{
    /*  RATING DA CBTM                 RATING DA FPOTM
    *    A ao E                              A
    *    F - G                               B
    *    H – I                               C
    *    J – L                               D
    *    M - N                               E
    *    O                                   F
    *    O (PRÉ-MIRIM E MIRIM)               G
    *                   MASCULINO
    */

    /*  RATING DA CBTM                 RATING DA FPOTM
    *    A ao E                              A
    *    F - G - H                           B
    *    I - J                               C
    *    J (PRÉ-MIRIM E MIRIM)               D
    *                   FEMININO
    */



    //Criando o Rating A
    categoriaInterestadual[0].categoria=" RATING A";
    int a=0;
    int soma=0;
    for(int i=n-1; i>=0; i--)
    {
        if(c[i].categoria=="RAM" || c[i].categoria=="RBM" || c[i].categoria=="RCM" || c[i].categoria=="RDM" || c[i].categoria=="REM")
        {
            categoriaInterestadual[0].categoria_cbtm[a]=c[i];
            soma=soma+c[i].quantidade_inscritos;
            a++;

            if(a==5)
            {
                break;
            }
        }
    }
    categoriaInterestadual[0].quantidade_inscritos=soma;
    categoriaInterestadual[0].quantidade_categorias=a;

    //criando Rating B
    categoriaInterestadual[1].categoria=" RATING B";
    a=0;
    soma=0;
    for(int i=n-1; i>=0; i--)
    {
        if(c[i].categoria=="RFM" || c[i].categoria=="RGM")
        {
            categoriaInterestadual[1].categoria_cbtm[a]=c[i];
            soma=soma+c[i].quantidade_inscritos;
            a++;
        }
        if(a==2)
        {
            break;
        }
    }
    categoriaInterestadual[1].quantidade_inscritos=soma;
    categoriaInterestadual[1].quantidade_categorias=a;


    //criando Rating C
    categoriaInterestadual[2].categoria=" RATING C";
    a=0;
    soma=0;
    for(int i=n-1; i>=0; i--)
    {
        if(c[i].categoria=="RHM" || c[i].categoria=="RIM")
        {
            categoriaInterestadual[2].categoria_cbtm[a]=c[i];
            soma=soma+c[i].quantidade_inscritos;
            a++;
        }

        if(a==2)
        {
            break;
        }
    }
    categoriaInterestadual[2].quantidade_inscritos=soma;
    categoriaInterestadual[2].quantidade_categorias=a;


    //criando Rating D
    categoriaInterestadual[3].categoria=" RATING D";
    a=0;
    soma=0;
    for(int i=n-1; i>=0; i--)
    {
        if(c[i].categoria=="RJM" || c[i].categoria=="RLM")
        {
            categoriaInterestadual[3].categoria_cbtm[a]=c[i];
            soma=soma+c[i].quantidade_inscritos;
            a++;
        }

        if(a==2)
        {
            break;
        }
    }
    categoriaInterestadual[3].quantidade_inscritos=soma;
    categoriaInterestadual[3].quantidade_categorias=a;

    //criando Rating E
    categoriaInterestadual[4].categoria=" RATING E";
    a=0;
    soma=0;
    for(int i=n-1; i>=0; i--)
    {
        if(c[i].categoria=="RMM" || c[i].categoria=="RNM")
        {
            categoriaInterestadual[4].categoria_cbtm[a]=c[i];
            soma=soma+c[i].quantidade_inscritos;
            a++;
        }

        if(a==2)
        {
            break;
        }
    }
    categoriaInterestadual[4].quantidade_inscritos=soma;
    categoriaInterestadual[4].quantidade_categorias=a;


    //criando Rating F
    categoriaInterestadual[5].categoria=" RATING F";
    soma=0;
    for(int i=0; i<=cont; i++)
    {
        if( (inscrito[i].ranking!="PRE MIRIM (MAS)" && inscrito[i].ranking!="SUPER PRE MIRIM (MAS)" && inscrito[i].ranking!="MIRIM (MAS)") && inscrito[i].rating=="ROM" && inscrito[i].participaRAT =="SIM")
        {
            soma=soma+1;
        }
    }
    if(soma>0)
    {
        categoriaInterestadual[5].categoria_cbtm[0].categoria="ROM";
        categoriaInterestadual[5].quantidade_categorias=1;
    }else
    {
        categoriaInterestadual[5].quantidade_categorias=0;
    }
    categoriaInterestadual[5].quantidade_inscritos=soma;

    //criando Rating G
    categoriaInterestadual[6].categoria=" RATING G";
    soma=0;
    for(int i=0; i<=cont; i++)
    {
        if((inscrito[i].ranking=="SUPER PRE MIRIM (MAS)" || inscrito[i].ranking=="PRE MIRIM (MAS)" || inscrito[i].ranking=="MIRIM (MAS)") && inscrito[i].rating=="ROM" && inscrito[i].participaRAT =="SIM")
        {
            soma=soma+1;
        }
    }
    if(soma>0)
    {
        categoriaInterestadual[6].categoria_cbtm[0].categoria="ROM";
        categoriaInterestadual[6].quantidade_categorias=1;
    }else
    {
        categoriaInterestadual[6].quantidade_categorias=0;
    }
    categoriaInterestadual[6].quantidade_inscritos=soma;

}

void listarCategoriasFpotm(CategoriasFpotm c[])
{
     cout<<endl<<"categorias do estadual:"<<endl;
     int soma=0;
    for(int i=0; i<7; i++)
    {
        if(c[i].quantidade_inscritos>0)
        {
            cout<<"==="<<c[i].categoria<<"==="<<endl;
            cout<<"Numero de inscritos:"<<c[i].quantidade_inscritos<<endl;
            soma+=c[i].quantidade_inscritos;
        }
    }

    cout<<"total: "<< soma<< " inscrito(s)."<<endl;
}





