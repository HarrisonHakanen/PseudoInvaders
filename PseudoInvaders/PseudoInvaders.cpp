#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::getline;
using std::string;

#include <vector>
using std::vector;

#include<array>
using std::array;

#include<windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <cctype>


struct Objeto {
    int id;
    int posicaoY;
    int posicaoX;
    char forma;
    boolean visible;
};


int criaMenu();
struct vector<Objeto> criaMonstros(vector<Objeto> objetos, int qtdMonstros, int qtdMonstrosPorLinha, int width);
struct Objeto atualizarPosicaoPersonagem(char tecla, struct Objeto personagem, int width, int height);
struct vector<Objeto> atualizarPosicaoTiros(struct vector<Objeto> objetos, int indexTiro);
struct vector<Objeto> atualizarPosicaoObjetos(char tecla, struct vector<Objeto> objetos, int width, int height);
void renderizarObjetos(vector<Objeto> objetos, int width, int height);
struct vector<Objeto> getPlayer(struct vector<Objeto> objetos);
struct vector<Objeto> atirar(char tecla, vector<Objeto> objetos);
struct vector<Objeto> acertouTiro(vector<Objeto> objetos);
boolean verificaGameOver(vector<Objeto> objetos);
int sistemaPontuacao(vector<Objeto> objetos, int pontuacao, int qtdMonstros);
vector<Objeto> inimigosLinhaDeFrente(vector<Objeto>objetos);
vector<Objeto> objetosInitial(int posicaoX, int posicaoY, int qtdMonstros, int qtdMonstrosPorLinha, int width);
struct vector<Objeto> atualizarPosicaoMonstros(struct vector<Objeto> objetos, int indexObj, int width);
struct vector<Objeto> eliminaObjetos(vector<Objeto>objetos);
struct vector<Objeto> tiroInimigo(vector<Objeto> objetos, vector<Objeto> inimigosQuePodemAtirar);
struct vector<Objeto> atualizarPosicaoTiroInimigo(struct vector<Objeto> objetos, int indexTiro, int height);
struct vector<Objeto> tiroContraTiro(vector<Objeto> objetos);
int tempo = 0;
boolean direita = false;



int main()
{
    //VARIÁVEIS PADRÕES
    int posicaoX = 18;
    int posicaoY = 25;
    int width = 50;
    int height = 20;
    char tecla;
    int escolha = 0;


    int qtdMonstros = 20;
    int qtdMonstrosPorLinha = 10;
    int pontuacao = 0;
    boolean gameOver = false;

    //OBJETOS EM ESTADO INICIAL
    vector<Objeto> objetos = objetosInitial(posicaoX, posicaoY, qtdMonstros, qtdMonstrosPorLinha, width);

    gameOver = verificaGameOver(objetos);


    while (true) {

        //CRIA MENU...
        if (escolha == 0 && !gameOver) {

            escolha = criaMenu();
        }

        //JOGO EM FUNCIONAMENTO
        if (escolha == 1) {

            if (!gameOver) {

                cout << "Pontuacao: ";
                cout << pontuacao;
                cout << "\n";

                renderizarObjetos(objetos, width, height);

                tecla = _getch();

                objetos = atualizarPosicaoObjetos(tecla, objetos, width, height);

                objetos = atirar(tecla, objetos);

                objetos = acertouTiro(objetos);

                pontuacao = sistemaPontuacao(objetos, pontuacao, qtdMonstros);

                vector<Objeto> inimigosQuePodemAtirar = inimigosLinhaDeFrente(objetos);

                tempo += 1;
                if (tempo == 4) {
                    tempo = 0;
                }

                objetos = tiroInimigo(objetos, inimigosQuePodemAtirar);

                objetos = tiroContraTiro(objetos);

                objetos = eliminaObjetos(objetos);

                system("cls");
            }

            else {

                for (int i = 0; i < height; i++) {

                    for (int j = 0; j < width; j++) {
                        if (i == height / 2 && j == width / 2) {


                            gameOver = false;
                            cout << "Game Over";
                            cout << "\n";
                            cout << "Sua pontuacao foi de: ";
                            cout << pontuacao;
                            cout << "\n\n";
                            system("pause");
                        }
                        cout << " ";
                    }
                    cout << "\n";
                }

                system("cls");
                escolha = criaMenu();
            }
        }
    }
    return 0;
}



struct vector<Objeto> tiroContraTiro(vector<Objeto> objetos) {

    for (int i = 0; i < objetos.size(); i++) {

        if (objetos[i].forma == 'i') {

            for (int j = 0; j < objetos.size(); j++) {

                if (objetos[j].forma == '|') {

                    if (objetos[i].posicaoX == objetos[j].posicaoX && objetos[i].posicaoY == objetos[j].posicaoY) {

                        objetos[i].visible = false;
                        objetos[j].visible = false;
                    }
                }
            }
        }
    }
    return objetos;
}


struct vector<Objeto> tiroInimigo(vector<Objeto> objetos, vector<Objeto> inimigosQuePodemAtirar) {

    for (int i = 0; i < objetos.size(); i++) {

        for (int j = 0; j < inimigosQuePodemAtirar.size(); j++) {

            if (objetos[i].id == inimigosQuePodemAtirar[j].id && objetos[i].forma == 'M') {
                if (tempo == 2) {
                    int ultimoId = objetos[objetos.size() - 1].id;
                    struct Objeto tiro = { ultimoId + 1,objetos[i].posicaoY + 1,objetos[i].posicaoX,'|',true };
                    objetos.push_back(tiro);
                }
            }
        }
    }

    return objetos;
}

struct vector<Objeto> eliminaObjetos(vector<Objeto>objetos) {

    for (int i = 0; i < objetos.size(); i++) {
        if (!objetos[i].visible) {
            objetos.erase(objetos.begin() + i);
        }
    }
    return objetos;
}


vector<Objeto> objetosInitial(int posicaoX, int posicaoY, int qtdMonstros, int qtdMonstrosPorLinha, int width) {

    vector<Objeto> objetos;
    struct Objeto personagem = { 0,posicaoX,posicaoY,'O',true };
    objetos.push_back(personagem);
    objetos = criaMonstros(objetos, qtdMonstros, qtdMonstrosPorLinha, width);

    return objetos;
}


vector<Objeto> inimigosLinhaDeFrente(vector<Objeto>objetos) {

    vector<Objeto>inimigosQuePodemAtirar;

    for (int i = 0; i < objetos.size(); i++) {

        if (objetos[i].forma == 'M') {

            boolean temInimigoAbaixo = false;

            for (int j = 0; j < objetos.size(); j++) {

                if (objetos[j].forma = 'M') {

                    if (objetos[i].posicaoX == objetos[j].posicaoX && objetos[i].posicaoY + 1 == objetos[j].posicaoY) {
                        temInimigoAbaixo = true;
                    }
                }
            }

            if (!temInimigoAbaixo) {
                inimigosQuePodemAtirar.push_back(objetos[i]);
            }
        }
    }

    return inimigosQuePodemAtirar;
}


int sistemaPontuacao(vector<Objeto> objetos, int pontuacao, int qtdMonstros) {

    int monstrosRestantes = 0;

    for (int i = 0; i < objetos.size(); i++) {
        if (objetos[i].forma == 'M') {
            monstrosRestantes += 1;
        }
    }
    pontuacao = qtdMonstros - monstrosRestantes;

    return pontuacao;
}

boolean verificaGameOver(vector<Objeto> objetos) {

    boolean gameOver = false;

    for (int i = 0; i < objetos.size(); i++) {

        if (objetos[i].forma = 'O') {

            for (int j = 0; j < objetos.size(); j++) {

                if (objetos[j].forma == 'M') {

                    if (objetos[i].posicaoX == objetos[j].posicaoX && objetos[i].posicaoY == objetos[j].posicaoY) {

                        gameOver = true;
                    }
                }
            }
        }
    }

    return gameOver;
}


struct vector<Objeto> acertouTiro(vector<Objeto> objetos) {

    for (int indexMonster = 0; indexMonster < objetos.size(); indexMonster++) {

        if (objetos[indexMonster].forma == 'M') {

            for (int indexTiro = 0; indexTiro < objetos.size(); indexTiro++) {

                if (objetos[indexTiro].forma == 'i') {

                    if (objetos[indexMonster].posicaoX == objetos[indexTiro].posicaoX && objetos[indexMonster].posicaoY == objetos[indexTiro].posicaoY) {

                        objetos[indexMonster].visible = false;
                        objetos[indexTiro].visible = false;
                    }
                }
            }
        }
    }
    return objetos;
}

struct vector<Objeto> atirar(char tecla, vector<Objeto> objetos) {

    if (tecla == 'j') {

        struct vector<Objeto> jogador = getPlayer(objetos);

        int ultimoId = objetos[objetos.size() - 1].id;
        struct Objeto tiro = { ultimoId + 1,jogador[0].posicaoY - 1,jogador[0].posicaoX,'i',true };
        objetos.push_back(tiro);
    }

    return objetos;
}


void renderizarObjetos(vector<Objeto> objetos, int width, int height) {

    boolean encontrou = false;

    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {
            for (int indexObj = 0; indexObj < objetos.size(); indexObj++) {

                if (objetos[indexObj].posicaoY == i && objetos[indexObj].posicaoX == j) {

                    cout << objetos[indexObj].forma;
                    encontrou = true;
                }
            }

            if (!encontrou) {
                cout << ' ';
            }
            encontrou = false;

        }
        cout << '\n';
    }
}

struct vector<Objeto> atualizarPosicaoObjetos(char tecla, struct vector<Objeto> objetos, int width, int height) {

    for (int indexObj = 0; indexObj < objetos.size(); indexObj++) {

        if (objetos[indexObj].forma == 'O') {

            objetos[indexObj] = atualizarPosicaoPersonagem(tecla, objetos[indexObj], width, height);
        }
        if (objetos[indexObj].forma == 'i') {

            objetos = atualizarPosicaoTiros(objetos, indexObj);
        }
        if (objetos[indexObj].forma == 'M') {

            objetos = atualizarPosicaoMonstros(objetos, indexObj, width);
        }

        if (objetos[indexObj].forma == '|') {

            objetos = atualizarPosicaoTiroInimigo(objetos, indexObj, height);
        }
    }
    return objetos;
}


struct vector<Objeto> atualizarPosicaoTiroInimigo(struct vector<Objeto> objetos, int indexTiro, int height) {

    if (objetos[indexTiro].posicaoY == height) {
        objetos[indexTiro].visible = false;
    }
    else {
        objetos[indexTiro].posicaoY += 1;
    }

    return objetos;
}

struct Objeto atualizarPosicaoPersonagem(char tecla, struct Objeto personagem, int width, int height) {

    tecla = toupper(tecla);

    if (tecla == 'A') {

        if (personagem.posicaoX - 1 >= 0) {
            personagem.posicaoX -= 1;
        }
    }
    if (tecla == 'D') {

        if (personagem.posicaoX + 1 < width) {
            personagem.posicaoX += 1;
        }
    }
    if (tecla == 'w') {

        if (personagem.posicaoY - 1 >= 0) {
            personagem.posicaoY -= 1;
        }
    }
    if (tecla == 's') {

        if (personagem.posicaoY + 1 < height) {
            personagem.posicaoY += 1;
        }
    }

    return personagem;

}

struct vector<Objeto> atualizarPosicaoTiros(struct vector<Objeto> objetos, int indexTiro) {

    if (objetos[indexTiro].posicaoY - 1 >= 0) {
        objetos[indexTiro].posicaoY -= 1;
    }
    else {
        objetos[indexTiro].visible = false;
    }
    return objetos;
}


struct vector<Objeto> atualizarPosicaoMonstros(struct vector<Objeto> objetos, int indexObj, int width) {

    if (objetos[indexObj].posicaoX == width) {
        direita = false;
        for (int indexMonster = 0; indexMonster < objetos.size(); indexMonster++) {

            if (objetos[indexMonster].forma == 'M') {
                objetos[indexMonster].posicaoY += 1;
            }
        }
    }

    if (objetos[indexObj].posicaoX == 0) {
        direita = true;
        for (int indexMonster = 0; indexMonster < objetos.size(); indexMonster++) {
            if (objetos[indexMonster].forma == 'M') {
                objetos[indexMonster].posicaoY += 1;
            }
        }
    }

    if (direita) {
        objetos[indexObj].posicaoX += 1;

    }
    else {
        objetos[indexObj].posicaoX -= 1;
    }

    return objetos;
}


int criaMenu() {

    cout << "Bem-vindo ao Pseudo Invaders!\n";
    cout << "Escolha uma opção:\n";
    cout << "1. Iniciar Jogo\n";
    cout << "2. Sair\n";

    int escolha;
    cin >> escolha;


    switch (escolha) {
    case 1:
        cout << "Iniciando o Jogo...\n";
        Sleep(2);
        system("cls");
        break;
    case 2:
        cout << "Saindo do Jogo...\n";
        return 0;
    default:
        cout << "Opção inválida. Tente novamente.\n";
    }

    return escolha;
}


struct vector<Objeto> getPlayer(struct vector<Objeto> objetos) {

    struct vector<Objeto> player;

    for (int i = 0; i < objetos.size(); i++) {
        if (objetos[i].forma == 'O') {
            player.push_back(objetos[i]);
        }
    }

    return player;
}


struct vector<Objeto> criaMonstros(vector<Objeto> objetos, int qtdMonstros, int qtdMonstrosPorLinha, int width) {

    float posXOriginal = width / 2;
    float posXMonster = posXOriginal;
    float posYMonster = 0;

    for (int i = 0; i < qtdMonstros; i++) {

        if (i % qtdMonstrosPorLinha == 0 && i != 0) {
            posYMonster += 1;
            posXMonster = posXOriginal;
        }

        int ultimoId = objetos[objetos.size() - 1].id;
        struct Objeto monstro = { ultimoId + 1,posYMonster,posXMonster,'M',true };
        objetos.push_back(monstro);
        posXMonster += 1;
    }

    return objetos;
}