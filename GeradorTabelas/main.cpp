/** \brief
 * purpose:Gerar grupos da fase classificatorios e seus respectivos confrontos.
           para as competições que utilizam as inscrições realizada na cbtm.org.br
           Para isso, este projeto requer que o arquivo .csv esteja com o nome "inscritos.csv"
 *
 * \author JOHN VICTOR ALVES LUIZ & GUILHERME DE MELO CARNEIRO
 * \version 1.0 31/05/2018
 */


#include <iostream>
#include <cstring>
#include <fstream>
#include <locale.h>
#include <string>
#include <sstream>
#include <cstdlib>

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
    int pontos;
};

//categorias gerais da CBTM
struct Categorias
{
    string categoria;
    int quantidade_inscritos;
};

// É necessario normalizar as categorias da CBTM para a categorias disponíveis do evento
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

struct Horario
{
    int hora;
    int minutos;
};


struct Confronto
{
    int id_confronto;
    Inscritos atleta01;
    Inscritos atleta02;
    string fase;
    Horario horario; //implementar depois os horarios que ocorrerão os jogos

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

struct Eliminatorias
{
    CategoriasFpotm categoria;
    Confronto confrontos[16];
    int quantidade_confrontos;

};



#define MAX_TXT 50
#define MAX_N   200

//RATINGS A à G
#define QUANTIDADE_RATINGS_MASCULINO_FPOTM 7
//RATINGS A à D
#define QUANTIDADE_RATINGS_FEMININO_FPOTM 4
//RANKINGS MASCULINOS
#define QUANTIDADE_RANKINGS_MASCULINO 16
//RANKINGS FEMININOS
#define QUANTIDADE_RANKINGS_FEMININO 15

//variavel global
//QUANTIDADE É A SOMA DE RATING E RANKINGS
int total= QUANTIDADE_RATINGS_MASCULINO_FPOTM + QUANTIDADE_RATINGS_FEMININO_FPOTM + QUANTIDADE_RANKINGS_FEMININO + QUANTIDADE_RANKINGS_FEMININO; // FALTA OS RANKINGS E O RATING FEMININO;




void detalharInscrito(Inscritos i);
int contarCategorias(Inscritos inscrito[], Categorias c[],int n);
void contarInscritos(Inscritos inscrito[], Categorias c[], int n, int qt);
void relatorioInscritos(Inscritos inscrito[], Categorias c[], int n, int qt);
void criarCategoriaFpotm(CategoriasFpotm categoriaInterestadual[], Inscritos inscrito[], Categorias c[],int n,int cont);
void definirCategoriaRanking(CategoriasFpotm &categoriaInterestadual, Inscritos inscrito[], int cont, string nomeCateg);
void listarCategoriasFpotm(CategoriasFpotm c[]);
void separarAtletasPorCategoriaFpotm(AtletasCategorias atletaCategoria[], CategoriasFpotm c[], Inscritos atleta[], int n, int qt);
void ordenarAtletasCategoriaCrescente(AtletasCategorias atleta[]);
void CriarGruposclassificatorios( AtletasCategorias atleta[], Grupo gp[]);
void ordenarAtletasGrupoCrescente(Grupo gp[], int t);
void listarGrupo(Grupo gp[], int t);
void criarConfrontos(Grupo gp[], int t);
void listarConfrontos(Grupo gp[], int t);
void confrontosEliminatorios(int quant_grupos);


int main()
{
    setlocale(LC_ALL, "Portuguese");

    int qt=0;

    Inscritos inscrito[100];
    Categorias c[100];
    CategoriasFpotm categoriaInterstadual[100];
    AtletasCategorias atletaCategoria[50];

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
                /*cout<<"!!!TECNICO!!!";
                 *não pegar quando for técnico e tecnico não possuem pontos de rating no momento;
                 *não pegar os pre-inscritos;
                 */
            }
            else
            {

                inscrito[qt]=t;
                inscrito[qt].pontos = atoi(inscrito[qt].ratingP.c_str());
                //stringstream ss(inscrito[qt].ratingP);
                //ss >> inscrito[qt].pontos;

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

    int cont=contarCategorias(inscrito,c,qt);

    contarInscritos(inscrito,c,cont,qt);
    //relatorioInscritos(inscrito,c,cont,qt-1);
    criarCategoriaFpotm(categoriaInterstadual,inscrito,c,cont,qt);

    //cout<<"---RELATÓRIO DE INSCRITOS FPOTM---"<<endl;

    //listarCategoriasFpotm(categoriaInterstadual);

    //cout<<"---Atletas separados por categoria(FPOTM)---"<<endl;
    separarAtletasPorCategoriaFpotm(atletaCategoria,categoriaInterstadual,inscrito,cont,qt);

    ordenarAtletasCategoriaCrescente(atletaCategoria);

    cout<<"=== Gerar Grupos e confrontos ==="<<endl;
    CriarGruposclassificatorios(atletaCategoria,grupos);

    return 0;
}

/** \brief
 *  Detalhar um inscrito(Atleta)
 * \param i Inscritos
 * \return void
 *
 */
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

/** \brief
 *  Contar as Categorias diferentes presente no .CSV
 * \param inscrito[] Inscritos
 * \param c[] Categorias
 * \param n int
 * \return int
 *
 */
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

/** \brief
 *  Calcular a quantidade de Inscritos do arquivo .CSV
 * \param inscrito[] Inscritos
 * \param c[] Categorias
 * \param n int
 * \param qt int
 * \return void
 *
 */
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
            else if(inscrito[i].rating==c[j].categoria && inscrito[i].participaRAT=="SIM")
            {
                a++;

            }
        }
        c[j].quantidade_inscritos=a;
    }
}


/** \brief
 *  Gerar um Relatório dos Inscritos do arquivo .CSV lido
 * \param inscrito[] Inscritos
 * \param c[] Categorias
 * \param n int
 * \param qt int
 * \return void
 *
 */
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


/** \brief
 *  Criar as Categorias Disponíveis pelas FPOTM
 *     RATING DA CBTM                 RATING DA FPOTM
    *    A ao E                              A
    *    F - G                               B
    *    H – I                               C
    *    J – L                               D
    *    M - N                               E
    *    O                                   F
    *    O (PRÉ-MIRIM E MIRIM)               G
    *                   MASCULINO
 *

 *     RATING DA CBTM                 RATING DA FPOTM
    *    A ao E                              A
    *    F - G - H                           B
    *    I - J                               C
    *    J (PRÉ-MIRIM E MIRIM)               D
    *                   FEMININO
 *
 * \param categoriaInterestadual[] CategoriasFpotm
 * \param inscrito[] Inscritos
 * \param c[] Categorias
 * \param n int
 * \param cont int
 * \return void
 *
 */
void criarCategoriaFpotm(CategoriasFpotm categoriaInterestadual[], Inscritos inscrito[], Categorias c[],int n,int cont)
{
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
    //Categorias Femininas

    //criando Rating A

    categoriaInterestadual[7].categoria ="RATING A (FEM)";
    a=0;
    soma=0;
    for(int i=n; i>=0; i--)
    {
        if(c[i].categoria=="RAF" || c[i].categoria=="RBF" || c[i].categoria=="RCF" || c[i].categoria=="RDF" || c[i].categoria=="REF")
        {
            categoriaInterestadual[7].categoria_cbtm[a]=c[i];
            soma=soma+c[i].quantidade_inscritos;
            a++;

            if(a==5)
            {
                break;
            }
        }
    }
    categoriaInterestadual[7].quantidade_inscritos=soma;
    categoriaInterestadual[7].quantidade_categorias=a;


    //criando Rating B

    categoriaInterestadual[8].categoria = "RATING B (FEM)";
    a=0;
    soma=0;
    for(int i=n; i>=0; i--)
    {
        if(c[i].categoria=="RFF"||c[i].categoria=="RGF"||c[i].categoria=="RHF")
        {
            categoriaInterestadual[8].categoria_cbtm[a]=c[i];
            soma=soma+c[i].quantidade_inscritos;
            a++;

            if(a==3)
            {
                break;
            }
        }
    }
    categoriaInterestadual[8].quantidade_inscritos=soma;
    categoriaInterestadual[8].quantidade_categorias=a;

    //criando Rating C

    categoriaInterestadual[9].categoria="RATING C (FEM)";
    soma=0;
    for(int i=0; i<=cont; i++)
    {
        if((((inscrito[i].ranking != "SUPER PRE MIRIM (FEM)"&&inscrito[i].ranking != "PRE MIRIM (FEM)"&&inscrito[i].ranking!="MIRIM (FEM)")&&inscrito[i].rating=="RJF"&&inscrito[i].participaRAT=="SIM"))||(inscrito[i].rating=="RIF"&&inscrito[i].participaRAT=="SIM"))
        {
            soma=soma+1;
        }
    }
    if(soma>0)
    {
        categoriaInterestadual[9].categoria_cbtm[0].categoria="RIF";
        categoriaInterestadual[9].categoria_cbtm[1].categoria="RJF";
        categoriaInterestadual[9].quantidade_categorias=2;
    }
    else
    {
        categoriaInterestadual[9].quantidade_categorias=0;
    }
    categoriaInterestadual[9].quantidade_inscritos=soma;

    //criando Rating D

    categoriaInterestadual[10].categoria="RATING D (FEM)";
    soma=0;
    for(int i=0; i<=cont; i++)
    {
        if((inscrito[i].ranking == "SUPER PRE MIRIM (FEM)" || inscrito[i].ranking == "PRE MIRIM (FEM)" || inscrito[i].ranking == "MIRIM (FEM)")&&inscrito[i].rating=="RJF"&&inscrito[i].participaRAT=="SIM")
        {
            soma=soma+1;
        }
    }
    if(soma>0)
    {
        categoriaInterestadual[10].categoria_cbtm[0].categoria="RJF";
        categoriaInterestadual[10].quantidade_categorias=1;
    }
    else
    {
        categoriaInterestadual[10].quantidade_categorias=0;
    }
    categoriaInterestadual[10].quantidade_inscritos=soma;

    //Criando RANKINGS

    //MASCULINO

    //Criando SUPER PRE MIRIM (MAS)
    string spmm = "SUPER PRE MIRIM (MAS)";
    definirCategoriaRanking(categoriaInterestadual[11], inscrito,cont,spmm);
    //Criando PRE MIRIM (MAS)
    string pmm = "PRE MIRIM (MAS)";
    definirCategoriaRanking(categoriaInterestadual[12], inscrito,cont,pmm);
    //Criando MIRIM (MAS)
    string mm="MIRIM (MAS)";
    definirCategoriaRanking(categoriaInterestadual[13], inscrito,cont,mm);
    //Criando INFANTIL (MAS)
    string im ="INFANTIL (MAS)";
    definirCategoriaRanking(categoriaInterestadual[14], inscrito,cont,im);
    //Criando JUVENIL (MAS)
    string jm ="JUVENIL (MAS)";
    definirCategoriaRanking(categoriaInterestadual[15], inscrito,cont,jm);
    //Criando JUVENTUDE (MAS)
    definirCategoriaRanking(categoriaInterestadual[16], inscrito,cont,"JUVENTUDE (MAS)");
    //Criando ABSOLUTO A (MAS)
    definirCategoriaRanking(categoriaInterestadual[17], inscrito,cont,"ABSOLUTO A (MAS)");
    //Criando ABSOLUTO B (MAS)
    definirCategoriaRanking(categoriaInterestadual[18], inscrito,cont,"ABSOLUTO B (MAS)");
    //Criando ABSOLUTO C (MAS)
    definirCategoriaRanking(categoriaInterestadual[19], inscrito,cont,"ABSOLUTO C (MAS)");
    //Criando ABSOLUTO D (MAS)
    definirCategoriaRanking(categoriaInterestadual[20], inscrito,cont,"ABSOLUTO D (MAS)");
    //Criando ABSOLUTO E (MAS)
    definirCategoriaRanking(categoriaInterestadual[21], inscrito,cont,"ABSOLUTO E (MAS)");
    //Criando SENIOR (MAS)
    definirCategoriaRanking(categoriaInterestadual[22], inscrito,cont,"SENIOR (MAS)");
    //Criando VETERANO 4 (MAS)
    definirCategoriaRanking(categoriaInterestadual[23], inscrito,cont,"VETERANO 4 (MAS)");
    //Criando VETERANO 5 (MAS)
    definirCategoriaRanking(categoriaInterestadual[24], inscrito,cont,"VETERANO 5 (MAS)");
    //Criando VETERANO 6 (MAS)
    definirCategoriaRanking(categoriaInterestadual[25], inscrito,cont,"VETERANO 6 (MAS)");
    //Criando VETERANO 7 (MAS)
    definirCategoriaRanking(categoriaInterestadual[26], inscrito,cont,"VETERANO 7 (MAS)");


    //FEMININO

    //Criando SUPER PRE MIRIM (FEM)
    definirCategoriaRanking(categoriaInterestadual[27], inscrito,cont,"SUPER PRE MIRIM (FEM)");
    //Criando PRE MIRIM (FEM)
    definirCategoriaRanking(categoriaInterestadual[28], inscrito,cont,"PRE MIRIM (FEM)");
    //Criando MIRIM (FEM)
    definirCategoriaRanking(categoriaInterestadual[29], inscrito,cont,"MIRIM (FEM)");
    //Criando INFANTIL (FEM)
    definirCategoriaRanking(categoriaInterestadual[30], inscrito,cont,"INFANTIL (FEM)");
    //Criando JUVENIL (FEM)
    definirCategoriaRanking(categoriaInterestadual[31], inscrito,cont,"JUVENIL (FEM)");
    //Criando JUVENTUDE (FEM)
    definirCategoriaRanking(categoriaInterestadual[32], inscrito,cont,"JUVENTUDE (FEM)");
    //Criando ABSOLUTO A (FEM)
    definirCategoriaRanking(categoriaInterestadual[33], inscrito,cont,"ABSOLUTO A (FEM)");
    //Criando ABSOLUTO B (FEM)
    definirCategoriaRanking(categoriaInterestadual[34], inscrito,cont,"ABSOLUTO B (FEM)");
    //Criando ABSOLUTO C (FEM)
    definirCategoriaRanking(categoriaInterestadual[35], inscrito,cont,"ABSOLUTO C (FEM)");
    //Criando ABSOLUTO D (FEM)
    definirCategoriaRanking(categoriaInterestadual[36], inscrito,cont,"ABSOLUTO D (FEM)");
    //Criando LADY (FEM)
    definirCategoriaRanking(categoriaInterestadual[37], inscrito,cont,"LADY (FEM)");
    //Criando VETERANO 4 (FEM)
    definirCategoriaRanking(categoriaInterestadual[38], inscrito,cont,"VETERANO 4 (FEM)");
    //Criando VETERANO 5 (FEM)
    definirCategoriaRanking(categoriaInterestadual[39], inscrito,cont,"VETERANO 5 (FEM)");
    //Criando VETERANO 6 (FEM)
    definirCategoriaRanking(categoriaInterestadual[40], inscrito,cont,"VETERANO 6 (FEM)");
    //Criando VETERANO 7 (FEM)
    definirCategoriaRanking(categoriaInterestadual[41], inscrito,cont,"VETERANO 7 (FEM)");


//
//	//MASCULINO
//
//	//Criando SUPER MIRIM (MAS)
//	definirCategoriaRanking(categoriaInterestadual[27], inscrito,cont,"SUPER MIRIM (MAS)");
//	//Criando SUPER INFANTIL (MAS)
//	definirCategoriaRanking(categoriaInterestadual[28], inscrito,cont,"SUPER INFANTIL (MAS)");
//	//Criando SUPER JUVENIL (MAS)
//	definirCategoriaRanking(categoriaInterestadual[29], inscrito,cont,"SUPER JUVENIL (MAS)");
//	//Criando INFANTIL ESPECIAL (MAS)
//	definirCategoriaRanking(categoriaInterestadual[30], inscrito,cont,"INFANTIL ESPECIAL (MAS)");
//	//Criando SUPER JUVENTUDE (MAS)
//	definirCategoriaRanking(categoriaInterestadual[31], inscrito,cont,"SUPER JUVENTUDE (MAS)");
//	//Criando ADULTO (MAS)
//	definirCategoriaRanking(categoriaInterestadual[32], inscrito,cont,"ADULTO (MAS)");
//	//Criando VETERANO 45 (MAS)
//	definirCategoriaRanking(categoriaInterestadual[33], inscrito,cont,"VETERANO 45 (MAS)");
//	//Criando VETERANO 55 (MAS)
//	definirCategoriaRanking(categoriaInterestadual[34], inscrito,cont,"VETERANO 55 (MAS)");
//	//Criando VETERANO 65 (MAS)
//	definirCategoriaRanking(categoriaInterestadual[35], inscrito,cont,"VETERANO 65 (MAS)");
//	//Criando ABSOLUTO C (MAS) - B
//	definirCategoriaRanking(categoriaInterestadual[36], inscrito,cont,"ABSOLUTO C (MAS) - B");
//	//Criando SENIOR (MAS) - B
//	definirCategoriaRanking(categoriaInterestadual[37], inscrito,cont,"SENIOR (MAS) - B");
//	//Criando MIRIM ESPECIAL (MAS)
//	definirCategoriaRanking(categoriaInterestadual[38], inscrito,cont,"MIRIM ESPECIAL (MAS)");
//	//Criando JUVENIL ESPECIAL (MAS)
//	definirCategoriaRanking(categoriaInterestadual[39], inscrito,cont,"JUVENIL ESPECIAL (MAS)");
//	//Criando JUVENTUDE ESPECIAL (MAS)
//	definirCategoriaRanking(categoriaInterestadual[40], inscrito,cont,"JUVENTUDE ESPECIAL (MAS)");
//	//Criando SENIOR (MAS)-CADEIRANTE
//	definirCategoriaRanking(categoriaInterestadual[41], inscrito,cont,"SENIOR (MAS)-CADEIRANTE");
//
//    //Feminino
//
//	//Criando JUVENTUDE ESPECIAL (FEM)
//	definirCategoriaRanking(categoriaInterestadual[57], inscrito,cont,"JUVENTUDE ESPECIAL (FEM)");
//	//Criando MIRIM (FEM) - B
//	definirCategoriaRanking(categoriaInterestadual[58], inscrito,cont,"PRE MIRIM (FEM) - B");
//	//Criando ADULTO (FEM)
//	definirCategoriaRanking(categoriaInterestadual[59], inscrito,cont,"ADULTO (FEM)");
//	//Criando MIRIM ESPECIAL (FEM)
//	definirCategoriaRanking(categoriaInterestadual[60], inscrito,cont,"MIRIM ESPECIAL (FEM)");
//	//Criando INFANTIL (FEM) - B
//	definirCategoriaRanking(categoriaInterestadual[61], inscrito,cont,"INFANTIL (FEM) - B");
//	//Criando INFANTIL ESPECIAL (FEM)
//	definirCategoriaRanking(categoriaInterestadual[62], inscrito,cont,"INFANTIL ESPECIAL (FEM)");
//	//Criando JUVENIL ESPECIAL (FEM)
//	definirCategoriaRanking(categoriaInterestadual[63], inscrito,cont,"JUVENIL ESPECIAL (FEM)");
//	//Criando VETERANO 45 (FEM)
//	definirCategoriaRanking(categoriaInterestadual[64], inscrito,cont,"VETERANO 45 (FEM)");
//	//Criando VETERANO 55 (FEM)
//	definirCategoriaRanking(categoriaInterestadual[65], inscrito,cont,"VETERANO 55 (FEM)");
//	//Criando VETERANO 65 (FEM)
//	definirCategoriaRanking(categoriaInterestadual[66], inscrito,cont,"VETERANO 65 (FEM)");
//	//Criando SUPER MIRIM (FEM)
//	definirCategoriaRanking(categoriaInterestadual[67], inscrito,cont,"SUPER MIRIM (FEM)");
//	//Criando SUPER INFANTIL (FEM)
//	definirCategoriaRanking(categoriaInterestadual[68], inscrito,cont,"SUPER INFANTIL (FEM)");
//	//Criando SUPER JUVENIL (FEM)
//	definirCategoriaRanking(categoriaInterestadual[69], inscrito,cont,"SUPER JUVENIL (FEM)");
//	//Criando SUPER JUVENTUDE (FEM)
//	definirCategoriaRanking(categoriaInterestadual[70], inscrito,cont,"SUPER JUVENTUDE (FEM)");

}

/** \brief
 *  Cria a categoria definida pela FPOTM
 * \param categoriaInterestadual CategoriasFpotm
 * \param inscrito[] Inscritos
 * \param cont int
 * \param nomeCateg string
 * \return void
 *
 */
void definirCategoriaRanking(CategoriasFpotm &categoriaInterestadual, Inscritos inscrito[], int cont, string nomeCateg)
{
    int soma=0;

    categoriaInterestadual.categoria_cbtm[0].categoria=nomeCateg;//correção, faltava


    for(int i=0; i<=cont; i++)
    {
        if(inscrito[i].ranking==nomeCateg)
        {
            soma=soma+1;
        }
    }
    if(soma>=0)
    {
        categoriaInterestadual.categoria=nomeCateg;
        categoriaInterestadual.quantidade_categorias=1;
    }
    else
    {
        categoriaInterestadual.quantidade_categorias=0;
    }

    categoriaInterestadual.quantidade_inscritos=soma;
}

/** \brief
 *  Listar as categorias criadas para a FPOTM
 * \param c[] CategoriasFpotm
 * \return void
 *
 */
void listarCategoriasFpotm(CategoriasFpotm c[])
{
    cout<<endl<<"categorias do estadual:"<<endl;
    int soma_RAT=0;
    int soma_RANK=0;
    for(int i=0; i<total-QUANTIDADE_RANKINGS_FEMININO-QUANTIDADE_RANKINGS_MASCULINO; i++)
    {
        if(c[i].quantidade_inscritos>0)
        {
            cout<<"==="<<c[i].categoria<<"==="<<endl;
            cout<<"Numero de inscritos:"<<c[i].quantidade_inscritos<<endl;
            soma_RAT+=c[i].quantidade_inscritos;

        }
    }
    for(int i=QUANTIDADE_RATINGS_FEMININO_FPOTM + QUANTIDADE_RATINGS_MASCULINO_FPOTM; i<total; i++)
    {
        if(c[i].quantidade_inscritos>0)
        {
            cout<<"==="<<c[i].categoria<<"==="<<endl;
            cout<<"Numero de inscritos:"<<c[i].quantidade_inscritos<<endl;
            soma_RANK+=c[i].quantidade_inscritos;

        }
    }

    cout<<"total Ranking: "<< soma_RANK<< " inscrito(s)."<<endl;
    cout<<"total Rating: "<< soma_RAT<< " inscrito(s)."<<endl;
}


/** \brief
 *  separar por categoria com os atletas para entrar na normalização
 * \param atletaCategoria[] AtletasCategorias
 * \param c[] CategoriasFpotm
 * \param atleta[] Inscritos
 * \param n int
 * \param qt int
 * \return void
 *
 */
void separarAtletasPorCategoriaFpotm(AtletasCategorias atletaCategoria[], CategoriasFpotm c[], Inscritos atleta[], int n, int qt)
{

    int cont=0;
    for(int i=0; i<total; i++)
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
                if(c[i].categoria_cbtm[j].categoria==atleta[k].rating && atleta[k].participaRAT=="SIM")
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
                    else if(c[i].categoria!="RATING F (MAS)" &&  c[i].categoria!="RATING G (MAS)")
                    {
                        if(c[i].categoria=="RATING A (FEM)"||c[i].categoria=="RATING B (FEM)"||c[i].categoria=="RATING C (FEM)"||c[i].categoria=="RATING D (FEM)")
                        {
                            if(c[i].categoria=="RATING C (FEM)"&& atleta[k].ranking!="SUPER PRE MIRIM (FEM)" && atleta[k].ranking!="PRE MIRIM (FEM)" && atleta[k].ranking!="MIRIM (FEM)")
                            {
                                cont++;
                                atletaCategoria[i].atleta[cont]=atleta[k];
                                cout<<"atleta:"<<atleta[k].atleta_nome<<" | "<<atleta[k].clube<<" | pontos RAT:"<<atleta[k].ratingP<<endl;
                            }
                            else if(c[i].categoria=="RATING D (FEM)"&&(atleta[k].ranking=="SUPER PRE MIRIM (FEM)" || atleta[k].ranking=="PRE MIRIM (FEM)" || atleta[k].ranking=="MIRIM (FEM)"))
                            {
                                cont++;
                                atletaCategoria[i].atleta[cont]=atleta[k];
                                cout<<"atleta:"<<atleta[k].atleta_nome<<" | "<<atleta[k].clube<<" | pontos RAT:"<<atleta[k].ratingP<<endl;
                            }
                            else if(c[i].categoria!="RATING C (FEM)"&&c[i].categoria!="RATING D (FEM)")
                            {
                                cont++;
                                atletaCategoria[i].atleta[cont]=atleta[k];
                                cout<<"atleta:"<<atleta[k].atleta_nome<<" | "<<atleta[k].clube<<" | pontos RAT:"<<atleta[k].ratingP<<endl;
                            }
                        }
                        else
                        {
                            cont++;
                            atletaCategoria[i].atleta[cont]=atleta[k];
                            cout<<"atleta:"<<atleta[k].atleta_nome<<" | "<<atleta[k].clube<<" | pontos RAT:"<<atleta[k].ratingP<<endl;
                        }

                    }

                }
                else  if(c[i].categoria_cbtm[j].categoria==atleta[k].ranking)
                {
                    cont++;
                    atletaCategoria[i].atleta[cont]=atleta[k];
                    cout<<"atleta:"<<atleta[k].atleta_nome<<" | "<<atleta[k].clube<<" | pontos RAT:"<<atleta[k].ratingP<<endl;
                }


            }

        }
        atletaCategoria[i].categoria_fpotm.quantidade_inscritos=cont;
        cout<<cont<<" atleta(s)!"<<endl<<endl;
    }



}

/** \brief
 *   Ordenar os atletas pela pontuação da CBTM para que na distribuição dos grupos seja escolhido os cabeça de chave
 *
 * \param atleta[] AtletasCategorias
 * \return void
 *
 */
void ordenarAtletasCategoriaCrescente(AtletasCategorias atleta[])
{



    AtletasCategorias aux;



    for(int i=0; i<total; i++)
    {

        aux.atleta[0].pontos=-1;

        for(int j=0; j<=atleta[i].categoria_fpotm.quantidade_inscritos; j++)
        {
            for(int k=0; k<atleta[i].categoria_fpotm.quantidade_inscritos; k++)
            {
                if(atleta[i].atleta[k].pontos>=atleta[i].atleta[k+1].pontos)
                {
                    aux.atleta[0]=atleta[i].atleta[k];
                    atleta[i].atleta[k]=atleta[i].atleta[k+1];
                    atleta[i].atleta[k+1]= aux.atleta[0];

                }


            }
        }

    }


}



/** \brief
 *  Criar os grupos da fase classificatória do evento
 * \param atleta[] AtletasCategorias
 * \param gp[] Grupo
 * \return void
 *
 */
void CriarGruposclassificatorios( AtletasCategorias atleta[], Grupo gp[])
{
    int numAtletaPorGrupo=3;
    int qt_atletas;
    int qt_grupos;
    char l[30] = {"ABCDEFGHIJKLMNOPQRSTUVXYZ"};

    //implementar a função para embaralhar os atletas


    for(int i=0; i <total; i++)
    {
        cout<<"categoria: "<<atleta[i].categoria_fpotm.categoria<< " com "<< atleta[i].categoria_fpotm.quantidade_inscritos<<" inscrito(s)"<<endl;

        qt_atletas=atleta[i].categoria_fpotm.quantidade_inscritos;

        numAtletaPorGrupo=3;

        if(qt_atletas==8)//exemplo de modelagem de atletas por grupo
        {
            numAtletaPorGrupo=4;
        }
        qt_grupos=qt_atletas/numAtletaPorGrupo; // 0 quando menor que trÊs... verificar

        int r=qt_atletas%numAtletaPorGrupo;//resta 1 ou 2 atletas
        int letra=0;

        if(qt_grupos>0)
        {



            for(int j=0; j<qt_grupos; j++)
            {
                gp[j].letra=l[letra];
                gp[j].categoria=atleta[i].categoria_fpotm;
                gp[j].quantidade_atletas=numAtletaPorGrupo;

                gp[j].atletas[0]=atleta[i].atleta[qt_atletas];

                qt_atletas--;
                letra++;

            }
            int a=1;
            for(int k=1; k<numAtletaPorGrupo && a<=qt_atletas ; k++)
            {
                letra=0;

                for(int j=0; j<qt_grupos; j++)
                {
                    gp[j].letra=l[letra];
                    gp[j].categoria=atleta[i].categoria_fpotm;
                    gp[j].quantidade_atletas=numAtletaPorGrupo;

                    gp[j].atletas[k]=atleta[i].atleta[a];

                    a++;
                    letra++;

                }



            }



            if(r==1)
            {


                gp[0].atletas[3]=atleta[i].atleta[qt_atletas];

                qt_atletas--;
                gp[0].quantidade_atletas++;


            }
            else if(r==2)
            {
                if(qt_grupos==4)
                {
                    for(int a=r-1; a>=0; a--)
                    {
                        gp[a].atletas[3]=atleta[i].atleta[qt_atletas];

                        qt_atletas--;
                        gp[a].quantidade_atletas++;

                    }
                }
                else
                {
                    gp[qt_grupos].atletas[0]=atleta[i].atleta[qt_atletas];

                    qt_atletas--;

                    gp[qt_grupos].atletas[1]=atleta[i].atleta[qt_atletas];

                    qt_atletas--;

                    gp[qt_grupos].quantidade_atletas=2;

                    qt_grupos++;
                }
            }



        }
        else if (r>0)
        {
            gp[0].letra=l[letra];
            gp[0].categoria=atleta[i].categoria_fpotm;
            gp[0].quantidade_atletas=r;
            qt_grupos=1;

            for(int a=0; a<r; a++)
            {
                gp[0].atletas[a]=atleta[i].atleta[qt_atletas];

                qt_atletas--;
            }

        }

        ordenarAtletasGrupoCrescente(gp,qt_grupos);
        listarGrupo(gp,qt_grupos);
        criarConfrontos(gp, qt_grupos);
        listarConfrontos(gp,qt_grupos);
        if(atleta[i].categoria_fpotm.quantidade_inscritos==0)
        {
            cout<<"nao possui inscritos!"<<endl;
        }
        cout<<endl;
    }


}

/** \brief Ordenar os atletas do grupo
    *   Ordena os atletas do grupo para criar os confrontos em seguida
    * \param gp[] Grupo
    * \param t int
    * \return void
    *
    */
void ordenarAtletasGrupoCrescente(Grupo gp[], int t)
{

    Grupo aux;


    for(int i=0; i<t; i++)
    {
        aux.atletas[0].pontos=-1;


        for(int j=0; j<gp[i].quantidade_atletas-1; j++)
        {
            for(int k=0; k<gp[i].quantidade_atletas-1; k++)
            {
                if(gp[i].atletas[k].pontos<=gp[i].atletas[k+1].pontos)
                {
                    aux.atletas[0]=gp[i].atletas[k];
                    gp[i].atletas[k]=gp[i].atletas[k+1];
                    gp[i].atletas[k+1]= aux.atletas[0];

                }


            }
        }



    }
}
/** \brief listar os atletas do grupo
 *
 * \param gp[] Grupo
 * \param t int
 * \return void
 *
 */
void listarGrupo(Grupo gp[], int t)
{
    for(int i=0; i<t; i++)
    {
        cout<<"Grupo "<<gp[i].letra<<endl;
        for(int j=0; j<=gp[i].quantidade_atletas-1; j++)
        {
            cout<<"atleta:"<<gp[i].atletas[j].atleta_nome<<"| pontos:"<<gp[i].atletas[j].pontos<<endl;

        }
    }
}

/** \brief Função para criar os confrontos do grupo
 *
 * \param gp[] Grupo
 * \param t int
 * \return void
 *
 */
void criarConfrontos(Grupo gp[], int t)
{
    cout<<endl<<"criando os confrontos... "<<endl;

    int n;

    for(int j=0; j<t; j++)
    {
        n=gp[j].quantidade_atletas;

        if (n==1)
        {
            //gp[j].confrontos[0].atleta01= gp[j].atletas[0];

            cout<<"não possui atleta suficiente..."<<endl;
            gp[j].quantidade_confrontos=0;

        }
        else if(n==2)
        {
            gp[j].confrontos[0].atleta01= gp[j].atletas[0];
            gp[j].confrontos[0].atleta02= gp[j].atletas[1];

            gp[j].quantidade_confrontos=1;

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
        else
        {
            cout<<"não possui inscritos!"<<endl;
        }



    }



}

/** \brief
 *  Função para listar os confrontos dos grupos
 * \param gp[] Grupo
 * \param t int
 * \return void
 *
 */
void listarConfrontos(Grupo gp[],int t)
{
    for(int j=0; j<t; j++)
    {
        if(gp[j].quantidade_confrontos==0)continue;
        cout<<endl<<"Confrontos do Grupo: "<<gp[j].letra<<endl;
        for(int i=0; i<gp[j].quantidade_confrontos; i++)
        {

            cout<<" " << gp[j].confrontos[i].atleta01.atleta_nome <<" x "<< gp[j].confrontos[i].atleta02.atleta_nome <<endl;

////
        }
        cout<<endl;
    }
    confrontosEliminatorios(t);
}

/** \brief
 *  modelagem dos cruzamentos da fase eliminatoria
 * \param quant_grupos int
 * \return void
 *
 */
void confrontosEliminatorios(int quant_grupos)
{

    cout<<"                             -----CONFRONTOS ELIMINATORIOS-----\n"<<endl;

    if(quant_grupos%2==0)
    {
        if(quant_grupos==2)
        {
            cout<<"\n\t\t-----SEMI-FINAIS-----"<<endl<<endl<<endl;
            cout<<"Jogo 01 -> "<<"| 1o do Grupo A |"<<" X "<<"| 2o do Grupo B |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 02 -> "<<"| 1o do Grupo B |"<<" X "<<"| 2o do Grupo B |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"\n\t\t-----FINAL-----"<<endl;
            cout<<"Jogo 03 -> "<<"| Vencedor 01 |"<<" X "<<"| Vencedor 02 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;

        }
        else if(quant_grupos==4)
        {
            cout<<"\n\t\t-----QUARTAS-DE-FINAL-----"<<endl<<endl<<endl;
            cout<<"Jogo 01 -> "<<"| 1o do Grupo A |"<<" X "<<"| 2o do Grupo C |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 02 -> "<<"| 1o do Grupo D |"<<" X "<<"| 2o do Grupo B |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 03 -> "<<"| 1o do Grupo B |"<<" X "<<"| 2o do Grupo D |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 04 -> "<<"| 1o do Grupo C |"<<" X "<<"| 2o do Grupo A |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"\n\t\t-----SEMI-FINAIS-----"<<endl<<endl<<endl;
            cout<<"Jogo 05 -> "<<"| Vencedor 01 |"<<" X "<<"| Vencedor 02 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 06 -> "<<"| Vencedor 03 |"<<" X "<<"| Vencedor 04 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"\n\t\t-----FINAL-----"<<endl<<endl<<endl;
            cout<<"Jogo 07 -> "<<"| Vencedor 05 |"<<" X "<<"| Vencedor 06 |"<<endl<<endl;

        }
        else if(quant_grupos==8)
        {
            cout<<"\n\t\t-----OITAVAS-DE-FINAL-----"<<endl<<endl<<endl;
            cout<<"Jogo 01 -> "<<"| 1o do Grupo A |"<<" X "<<"| 2o do Grupo H |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 02 -> "<<"| 1o do Grupo F |"<<" X "<<"| 2o do Grupo C |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 03 -> "<<"| 1o do Grupo G |"<<" X "<<"| 2o do Grupo B |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 04 -> "<<"| 1o do Grupo D |"<<" X "<<"| 2o do Grupo E |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 05 -> "<<"| 1o do Grupo E |"<<" X "<<"| 2o do Grupo D |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 06 -> "<<"| 1o do Grupo B |"<<" X "<<"| 2o do Grupo G |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 07 -> "<<"| 1o do Grupo C |"<<" X "<<"| 2o do Grupo F |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 08 -> "<<"| 1o do Grupo H |"<<" X "<<"| 2o do Grupo A |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"\n\t\t-----QUARTAS-DE-FINAL-----"<<endl<<endl<<endl;
            cout<<"Jogo 09 -> "<<"| Vencedor 01 |"<<" X "<<"| Vencedor 02 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 10 -> "<<"| Vencedor 03 |"<<" X "<<"| Vencedor 04 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 11 -> "<<"| Vencedor 05 |"<<" X "<<"| Vencedor 06 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 12 -> "<<"| Vencedor 07 |"<<" X "<<"| Vencedor 08 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"\n\t\t-----SEMI-FINAIS-----"<<endl<<endl<<endl;
            cout<<"Jogo 13 -> "<<"| Vencedor 09 |"<<" X "<<"| Vencedor 10 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 14 -> "<<"| Vencedor 11 |"<<" X "<<"| Vencedor 12 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"\n\t\t-----FINAL-----"<<endl<<endl<<endl;
            cout<<"Jogo 15 -> "<<"| Vencedor 13 |"<<" X "<<"| Vencedor 14 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
        }
        else if(quant_grupos==16)
        {
            cout<<"\n\t\t-----CHAVES-----"<<endl<<endl<<endl;
            cout<<"Jogo 01 -> "<<"| 1o do Grupo A |"<<" X "<<"| 2o do Grupo P |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 02 -> "<<"| 1o do Grupo E |"<<" X "<<"| 2o do Grupo L |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 03 -> "<<"| 1o do Grupo J |"<<" X "<<"| 2o do Grupo G |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 04 -> "<<"| 1o do Grupo N |"<<" X "<<"| 2o do Grupo C |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 05 -> "<<"| 1o do Grupo D |"<<" X "<<"| 2o do Grupo M |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 06 -> "<<"| 1o do Grupo K |"<<" X "<<"| 2o do Grupo F |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 07 -> "<<"| 1o do Grupo H |"<<" X "<<"| 2o do Grupo I |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 08 -> "<<"| 1o do Grupo O |"<<" X "<<"| 2o do Grupo B |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 09 -> "<<"| 1o do Grupo B |"<<" X "<<"| 2o do Grupo O |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 10 -> "<<"| 1o do Grupo I |"<<" X "<<"| 2o do Grupo H |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 11 -> "<<"| 1o do Grupo F |"<<" X "<<"| 2o do Grupo K |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 12 -> "<<"| 1o do Grupo M |"<<" X "<<"| 2o do Grupo D |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 13 -> "<<"| 1o do Grupo C |"<<" X "<<"| 2o do Grupo N |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 14 -> "<<"| 1o do Grupo G |"<<" X "<<"| 2o do Grupo J |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 15 -> "<<"| 1o do Grupo L |"<<" X "<<"| 2o do Grupo E |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 16 -> "<<"| 1o do Grupo P |"<<" X "<<"| 2o do Grupo A |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"\n\t\t-----OITAVAS-DE-FINAL-----"<<endl<<endl<<endl;
            cout<<"Jogo 17 -> "<<"| Vencedor 01 |"<<" X "<<"| Vencedor 02 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 18 -> "<<"| Vencedor 03 |"<<" X "<<"| Vencedor 04 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 19 -> "<<"| Vencedor 05 |"<<" X "<<"| Vencedor 06 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 20 -> "<<"| Vencedor 07 |"<<" X "<<"| Vencedor 08 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 21 -> "<<"| Vencedor 09 |"<<" X "<<"| Vencedor 10 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 22 -> "<<"| Vencedor 11 |"<<" X "<<"| Vencedor 12 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 23 -> "<<"| Vencedor 13 |"<<" X "<<"| Vencedor 14 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 24 -> "<<"| Vencedor 15 |"<<" X "<<"| Vencedor 16 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"\n\t\t-----QUARTAS-DE-FINAL-----"<<endl<<endl<<endl;
            cout<<"Jogo 25 -> "<<"| Vencedor 17 |"<<" X "<<"| Vencedor 18 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 26 -> "<<"| Vencedor 19 |"<<" X "<<"| Vencedor 20 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 27 -> "<<"| Vencedor 21 |"<<" X "<<"| Vencedor 22 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 28 -> "<<"| Vencedor 23 |"<<" X "<<"| Vencedor 24 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"\n\t\t-----SEMI-FINAIS-----"<<endl<<endl<<endl;
            cout<<"Jogo 29 -> "<<"| Vencedor 25 |"<<" X "<<"| Vencedor 26 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 30 -> "<<"| Vencedor 27 |"<<" X "<<"| Vencedor 28 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"\n\t\t-----FINAL-----"<<endl<<endl<<endl;
            cout<<"Jogo 31 -> "<<"| Vencedor 29 |"<<" X "<<"| Vencedor 30 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
        }
    }
    else
    {
        if(quant_grupos==1)
        {
            cout<<"\n\t\t-----FINAL-----"<<endl;
            cout<<"Jogo 01 -> "<<"| 1o do Grupo A |"<<" X "<<"| 2o do Grupo A |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;

        }else if(quant_grupos==3)
        {
            cout<<"\n\t\t-----QUARTAS-DE-FINAL-----"<<endl<<endl<<endl;
            cout<<"Jogo 01 -> "<<"| 2o do Grupo B |"<<" X "<<"| 2o do Grupo C |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 02 -> "<<"| 1o do Grupo C |"<<" X "<<"| 2o do Grupo A |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"\n\t\t-----SEMI-FINAIS-----"<<endl<<endl<<endl;
            cout<<"Jogo 03 -> "<<"| Vencedor 01 |"<<" X "<<"| 1o do Grupo A |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 04 -> "<<"| Vencedor 02 |"<<" X "<<"| 1o do Grupo B |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"\n\t\t-----FINAL-----"<<endl<<endl<<endl;
            cout<<"Jogo 31 -> "<<"| Vencedor 03 |"<<" X "<<"| Vencedor 04 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
        }
        else if(quant_grupos==5)
        {
            cout<<"\n\t\t-----OITAVAS-DE-FINAL-----"<<endl<<endl<<endl;
            cout<<"Jogo 01 -> "<<"| 2o do Grupo B |"<<" X "<<"| 2o do Grupo C |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"Jogo 02 -> "<<"| 2o do Grupo A |"<<" X "<<"| 2o do Grupo D |"<<endl<<endl;
            cout<<"          "<<"__________________ "<<"X"<<" __________________"<<endl<<endl;
            cout<<"\n\t\t-----QUARTAS-DE-FINAIS-----"<<endl<<endl<<endl;
            cout<<"Jogo 03 -> "<<"| Vencedor 01 |"<<" X "<<"| 1o do Grupo A |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 04 -> "<<"| 1o do Grupo D |"<<" X "<<"| 1o do Grupo E |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 05 -> "<<"| Vencedor 02 |"<<" X "<<"| 1o do Grupo B |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 06 -> "<<"| 1o do Grupo C |"<<" X "<<"| 2o do Grupo E |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"\n\t\t-----SEMI-FINAIS-----"<<endl<<endl<<endl;
            cout<<"Jogo 07 -> "<<"| Vencedor 03 |"<<" X "<<"| Vencedor 04 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"Jogo 08 -> "<<"| Vencedor 05 |"<<" X "<<"| Vencedor 06 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;
            cout<<"\n\t\t-----FINAL-----"<<endl<<endl<<endl;
            cout<<"Jogo 09 -> "<<"| Vencedor 07 |"<<" X "<<"| Vencedor 08 |"<<endl<<endl;
            cout<<"         "<<"_________________ "<<"X"<<" _________________"<<endl<<endl;

        }
    }
}
