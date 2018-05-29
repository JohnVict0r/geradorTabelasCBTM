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
    Categorias categoria_cbtm[5];
    int quantidade_inscritos;
    int quantidade_categorias;

};

//preparando para criar os grupos por categoria
struct AtletasCategorias
{
    CategoriasFpotm categoria_fpotm;
    Inscritos atleta[100];

};

struct Confronto
{
    Inscritos atleta01;
    Inscritos atleta02;
    string fase;

};

struct Grupo
{
    char letra;
    Inscritos atletas[6];
    CategoriasFpotm categoria;
    Confronto confrontos[20];
    int quantidade_confrontos;
    int quantidade_atletas;

};



#define MAX_TXT 50
#define MAX_N   200

//RATINGS A à G
#define QUANTIDADE_RATINGS_MASCULINO_FPOTM 7
//RATINGS A à D
#define QUANTIDADE_RATINGS_FEMININO_FPOTM 4


//QUANTIDADE É A SOMA DE RATING E RANKINGS
int t= QUANTIDADE_RATINGS_MASCULINO_FPOTM; // FALTA OS RANKINGS E O RATING FEMININO;
//variavel global



void detalharInscrito(Inscritos i);
int contarCategorias(Inscritos inscrito[], Categorias c[],int n);
void contarInscritos(Inscritos inscrito[], Categorias c[], int n, int qt);
void relatorioInscritos(Inscritos inscrito[], Categorias c[], int n, int qt);
void criarCategoriaRatingFpotm(CategoriasFpotm categoriaInterestadual[], Inscritos inscrito[], Categorias c[],int n,int cont);
void listarCategoriasFpotm(CategoriasFpotm c[]);
void separarAtletasPorCategoriaFpotm(AtletasCategorias atletaCategoria[], CategoriasFpotm c[], Inscritos atleta[], int n, int qt);
void CriarGruposclassificatorios( AtletasCategorias atleta[], Grupo gp[]);
void criarConfrontos(Grupo gp[], int t);
void listarConfrontos(Grupo gp[], int t);


int main()
{
    setlocale(LC_ALL, "Portuguese");

    //char nomes[MAX_N][MAX_TXT];
    //char arquivo[MAX_TXT];
    //int n;
    int qt=0;

    Inscritos inscrito[100];
    Categorias c[100];
    CategoriasFpotm categoriaInterstadual[100];
    AtletasCategorias atletaCategoria[20];

    Grupo grupos[100];


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



    contarInscritos(inscrito,c,cont,qt-1);
    //relatorioInscritos(inscrito,c,cont,qt-1);
    criarCategoriaRatingFpotm(categoriaInterstadual,inscrito,c,cont,qt-1);

    //cout<<"---RELATÓRIO DE INSCRITOS FPOTM---"<<endl;


    //listarCategoriasFpotm(categoriaInterstadual);

    cout<<"---Atletas separados por categoria(FPOTM)---"<<endl;
    separarAtletasPorCategoriaFpotm(atletaCategoria,categoriaInterstadual,inscrito,cont,qt-1);

    cout<<"=== 'Sorteio' ==="<<endl;
    CriarGruposclassificatorios(atletaCategoria,grupos);

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

void contarInscritos(Inscritos inscrito[], Categorias c[], int n, int qt)
{
    /*
    * conta os inscritos
    */

    int a=0;
    for(int j=0; j<=n; j++)
    {
        a=0;
        for(int i=1; i<=qt; i++)
        {
            if(inscrito[i].ranking==c[j].categoria)
            {
                a++;

            }
            if(inscrito[i].rating==c[j].categoria && inscrito[i].participaRAT=="SIM")
            {
                a++;

            }
        }
        c[j].quantidade_inscritos=a;
    }
}


void relatorioInscritos(Inscritos inscrito[], Categorias c[], int n, int qt)
{
    /*
    * emite o relatório dos inscritos por categoria
    */

    for(int j=0; j<=n; j++)
    {
        cout<<"==="<<c[j].categoria<<"==="<<endl;
        for(int i=1; i<=qt; i++)
        {
            if(inscrito[i].ranking==c[j].categoria)
            {

                cout<<"atleta: "<<inscrito[i].clube<<" | "<<inscrito[i].atleta_nome<<" | pontos:"<<inscrito[i].ratingP<<endl;

            }
            if(inscrito[i].rating==c[j].categoria && inscrito[i].participaRAT=="SIM")
            {

                cout<<"atleta: "<<inscrito[i].clube<<" | "<<inscrito[i].atleta_nome<<" | pontos:"<<inscrito[i].ratingP <<" | Ranking:"<<inscrito[i].ranking<<endl;

            }


        }
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
    categoriaInterestadual[0].categoria="RATING A (MAS)";
    int a=0;
    int soma=0;
    for(int i=n; i>=0; i--)
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
    categoriaInterestadual[1].categoria="RATING B (MAS)";
    a=0;
    soma=0;
    for(int i=n; i>=0; i--)
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
    categoriaInterestadual[2].categoria="RATING C (MAS)";
    a=0;
    soma=0;
    for(int i=n; i>=0; i--)
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
    categoriaInterestadual[3].categoria="RATING D (MAS)";
    a=0;
    soma=0;
    for(int i=n; i>=0; i--)
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
    categoriaInterestadual[4].categoria="RATING E (MAS)";
    a=0;
    soma=0;
    for(int i=n; i>=0; i--)
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
    categoriaInterestadual[5].categoria="RATING F (MAS)";
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
    }
    else
    {
        categoriaInterestadual[5].quantidade_categorias=0;
    }
    categoriaInterestadual[5].quantidade_inscritos=soma;

    //criando Rating G
    categoriaInterestadual[6].categoria="RATING G (MAS)";
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
    }
    else
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

void separarAtletasPorCategoriaFpotm(AtletasCategorias atletaCategoria[], CategoriasFpotm c[], Inscritos atleta[], int n, int qt)
{

    int cont=0;
    for(int i=0; i<t; i++)
    {
        atletaCategoria[i].categoria_fpotm.categoria=c[i].categoria;
        if(c[i].quantidade_inscritos==0)
            continue;
        cout<<"atletas da categoria: "<<c[i].categoria<<endl;

        cont=0;
        for(int j=0; j<c[i].quantidade_categorias; j++)
        {
            for(int k=0; k<qt; k++)
            {
                if((c[i].categoria_cbtm[j].categoria==atleta[k].rating && atleta[k].participaRAT=="SIM") || c[i].categoria_cbtm[j].categoria==atleta[k].ranking)
                {
                    //tratando os casos especiais, devido a nao conseguir distinguir apenas pelo rating!
                    if( c[i].categoria=="RATING F (MAS)" && atleta[k].ranking!="SUPER PRE MIRIM (MAS)" && atleta[k].ranking!="PRE MIRIM (MAS)" && atleta[k].ranking!="MIRIM (MAS)")
                    {
                        cont++;
                        atletaCategoria[i].atleta[cont]=atleta[k];

                        cout<<"atleta:"<<atleta[k].atleta_nome<<" | "<<atleta[k].clube<<" | pontos RAT:"<<atleta[k].ratingP<<endl;

                    }
                    else if( c[i].categoria=="RATING G (MAS)" && (atleta[k].ranking=="SUPER PRE MIRIM (MAS)" || atleta[k].ranking=="PRE MIRIM (MAS)" || atleta[k].ranking=="MIRIM (MAS)"))
                    {
                        cont++;
                        atletaCategoria[i].atleta[cont]=atleta[k];
                        cout<<"atleta:"<<atleta[k].atleta_nome<<" | "<<atleta[k].clube<<" | pontos RAT:"<<atleta[k].ratingP<<endl;

                    }
                    else if(c[i].categoria!="RATING F (MAS)" &&  c[i].categoria!="RATING G (MAS)" )
                    {
                        cont++;
                        atletaCategoria[i].atleta[cont]=atleta[k];
                        cout<<"atleta:"<<atleta[k].atleta_nome<<" | "<<atleta[k].clube<<" | pontos RAT:"<<atleta[k].ratingP<<endl;

                    }

                }

            }

        }
        atletaCategoria[i].categoria_fpotm.quantidade_inscritos=cont;
        cout<<cont<<" atleta(s)!"<<endl<<endl;
    }



}

void CriarGruposclassificatorios( AtletasCategorias atleta[], Grupo gp[])
{

    int qt_atletas;
    int qt_grupos;
    char l[30] = {"ABCDEFGHIJKLMNOPQRSTUVXYZ"};

    //implementar a função para embaralhar os atletas


    for(int i=0; i <t; i++)
    {
        cout<<"categoria: "<<atleta[i].categoria_fpotm.categoria<< " com "<< atleta[i].categoria_fpotm.quantidade_inscritos<<" inscrito(s)"<<endl;

        qt_atletas=atleta[i].categoria_fpotm.quantidade_inscritos;
        qt_grupos=qt_atletas/3; // 0 quando menor que trÊs... verificar
        int r=qt_atletas%3;//resta 1 ou 2 atletas
        int letra=0;

        if(qt_grupos>0)
        {


            for(int j=0; j<qt_grupos; j++)
            {
                gp[j].letra=l[letra];
                gp[j].categoria=atleta[i].categoria_fpotm;
                gp[j].quantidade_atletas=3;

                cout<<"Grupo "<< l[letra]<<endl;

                for(int k=0; k<3 ; k++)
                {
                    gp[j].atletas[k]=atleta[i].atleta[qt_atletas];

                    cout<<"atleta:"<<gp[j].atletas[k].atleta_nome<<endl;

                    qt_atletas--;

                }
                letra++;


            }
            if(r>0)
            {
                for(int a=0; a<r; a++)
                {
                    cout<<"(Acrescentando) Grupo "<< l[a]<<endl;
                    gp[a].atletas[3]=atleta[i].atleta[qt_atletas];
                    cout<<"atleta:"<<atleta[i].atleta[qt_atletas].atleta_nome<<endl;
                    qt_atletas--;
                    gp[a].quantidade_atletas++;

                }
            }



        }
        else if (r>0)
        {
            gp[0].letra=l[letra];
            gp[0].categoria=atleta[i].categoria_fpotm;
            gp[0].quantidade_atletas=r;
            qt_grupos=1;

            cout<<"Grupo "<< l[letra]<<endl;
            for(int a=0; a<r; a++)
            {
                gp[0].atletas[a]=atleta[i].atleta[qt_atletas];
                cout<<"atleta:"<<atleta[i].atleta[qt_atletas].atleta_nome<<endl;
                qt_atletas--;


            }

        }

        criarConfrontos(gp, qt_grupos);
        listarConfrontos(gp,qt_grupos);

        cout<<endl;
    }


}

//Criar função para ordenar atleta por pontos dentro dos grupos.;

void criarConfrontos(Grupo gp[], int t)
{
    cout<<"criando os confrontos: "<<endl;

    int n;

    for(int j=0; j<t; j++)
    {
        n=gp[j].quantidade_atletas;

        if(n==2)
        {
            gp[j].confrontos[0].atleta01= gp[j].atletas[0];
            gp[j].confrontos[0].atleta02= gp[j].atletas[1];

        }
        else if(n==3)
        {
            gp[j].confrontos[0].atleta01= gp[j].atletas[0];
            gp[j].confrontos[0].atleta02= gp[j].atletas[2];

            gp[j].confrontos[1].atleta01= gp[j].atletas[1];
            gp[j].confrontos[1].atleta02= gp[j].atletas[2];

            gp[j].confrontos[2].atleta01= gp[j].atletas[0];
            gp[j].confrontos[2].atleta02= gp[j].atletas[1];

            gp[j].quantidade_confrontos=3;

        }
        else if(n=4)
        {

            gp[j].confrontos[0].atleta01= gp[j].atletas[0];
            gp[j].confrontos[0].atleta02= gp[j].atletas[3];

            gp[j].confrontos[1].atleta01= gp[j].atletas[1];
            gp[j].confrontos[1].atleta02= gp[j].atletas[2];

            gp[j].confrontos[2].atleta01= gp[j].atletas[0];
            gp[j].confrontos[2].atleta02= gp[j].atletas[2];

            gp[j].confrontos[3].atleta01= gp[j].atletas[1];
            gp[j].confrontos[3].atleta02= gp[j].atletas[3];

            gp[j].confrontos[4].atleta01= gp[j].atletas[0];
            gp[j].confrontos[4].atleta02= gp[j].atletas[1];

            gp[j].confrontos[5].atleta01= gp[j].atletas[2];
            gp[j].confrontos[5].atleta02= gp[j].atletas[3];

            gp[j].quantidade_confrontos=6;

        }
        else if(n=5)
        {

        }
        else if(n=6)
        {

        }


    }



}

void listarConfrontos(Grupo gp[],int t)
{
    for(int j=0; j<t; j++)
    {

        for(int i=0; i<gp[j].quantidade_confrontos; i++)
        {

            cout<< gp[j].letra <<" | " << gp[j].confrontos[i].atleta01.atleta_nome <<" x "<< gp[j].confrontos[i].atleta02.atleta_nome <<endl;


        }
    }
}
