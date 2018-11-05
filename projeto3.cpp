// Universidade de Brasília
// Ciência da Computação
// Disciplina: Teoria e Aplicação de Grafos
// Professor: Dibio Leandro Borges
// Período: 2018/2
// Projeto 3
// Aluno:
//     Matheus Rodrigues Guimarães - 15/0141661

#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <map>

using namespace std;
class Graph{
    map<list<int>, list<string> > professores;
    map<string, list<int> > escolas_contratantes;
    map<list<int>, string> assignments_professores;
    map<string, list<list<int> > > assignments_escolas;
    int total_vagas = 0 , total_professores = 0;

    public:
    //funcao que le do arquivo e faz o parse
    void inicializaMaps(){
        string line, number, escola;
        list<int> professor, info_escola;
        list<string> escolas;
        ifstream file("entrada.txt");
        int i, j, prof_number, habilitacao, vagas, aux, esc = 0;
        char char_aux;

        while (getline(file, line)){
            line.push_back('\n');
            if(line.size() <= 36 && line.size() > 18){ 
                i=2;
                j=0;
                while(line[i] != ','){
                    number[j] = line[i];
                    j++;
                    i++;
                }
                prof_number = stoi(number);
                professor.push_back(prof_number);
                total_professores+=1;
                i+=2;
                number = line[i];
                habilitacao = stoi(number);
                // cout << habilitacao << "\n" << endl;
                professor.push_back(habilitacao);
                assignments_professores[professor] = "";

                // cout << prof_number << " " << habilitacao << "\n";

                i+=5;
                j=0;
                aux = i;

                do{
                    // cout << line[i] << "\n";
                    if(line[i] != ',' && line[i] != ')'){
                        // escola[j] += line[i];
                        i++;
                        j++;
                        // cout << i << '\n';
                    }
                    else{
                        escola = line.substr(aux, j);
                        // cout << escola << "\n";
                        escolas.push_back(escola);
                        escola.clear();
                        i+=2;
                        j=0;
                        aux = i;
                    }
                    if(i>line.size()-1){
                        i = line.size()-1;
                    }
                }while(line[i] != '\n');

                // for (auto v : escolas){
                //     cout << v << " ";
                // }

                professores[professor] = escolas;

                // cout << "\n";
                professor.clear();
                escolas.clear();
            }
            else if(line.size() < 19){
                i=1;
                j=0;
                while(line[i] != '\n'){
                    if(line[i] != ')'){
                        j++;
                        if(j > 4){
                            j = 2;
                        }
                    }
                    else{
                        escola = line.substr(1, j);
                        esc++;
                        assignments_escolas[escola] = {};
                        i+=3;
                        number = line.substr(i, 1);
                        habilitacao = stoi(number);
                        info_escola.push_back(habilitacao);

                        i+=4;
                        number = line.substr(i, 1);
                        
                        if(esc < 10){
                            vagas = 1;
                        }
                        else{
                            vagas = 2;
                        }
                        vagas = stoi(number);
                        if(i > 10){
                            i=16;
                            number = line[i-1];
                            vagas = stoi(number);
                            i--;
                        }
                        total_vagas+=vagas;
                        info_escola.push_back(vagas);

                        escolas_contratantes[escola] = info_escola;
                        info_escola.clear();
                        i++;
                    }
                    i++;
                }
            }
        }

        // cout << professores.size() << "\n";
        // cout << escolas_contratantes.size() << "\n";
        // cout << total_professores << "\n";
        // cout << total_vagas << "\n";
        // cout << assignments_professores.size() << "\n";
        // cout << assignments_escolas.size() << "\n";
    }

    void assignmentsArrangement(){
        map<string, list<int> >::iterator imprime_emparelhamento;
        map<list<int>, list<string> >::iterator it;
        map<list<int>, string>::iterator print_assignment_prof;
        map<string, list<list<int> > >::iterator print_assignment_escolas;
        list<list<int> >::iterator iter, get_professores;
        list<int> professor_avaliado;
        string escola_escolhida;
        bool trocar = true;
        int i=0;    

        //o método de emparelhamento aplicado leva em consideração que caso não seja possível preencher a vaga da escola com um professor com a quantidade de habilitações solicitadas
        //haverá preenchimento de vaga com outro professor; válido lembrar que o algorítmo favorece os professores e suas escolhas;

        //loop para iterar entre todas as opções de escolas dos professores
        while(!professores.begin()->second.empty() && total_professores > 0){
            for (it = professores.begin(); (it != professores.end()) && (!it->second.empty()); it++){
                professor_avaliado = it->first;
                escola_escolhida = it->second.front();
                it->second.pop_front();

                //verifica se o professor está livre e se existe vaga na escola avaliada 
                if(assignments_professores[professor_avaliado] == "" && assignments_escolas[escola_escolhida].size() < escolas_contratantes[escola_escolhida].back()){
                    assignments_professores[professor_avaliado] = escola_escolhida;
                    assignments_escolas[escola_escolhida].push_front(professor_avaliado);
                    total_vagas--;
                    total_professores--;
                }
                //se não houver vaga na escola avaliada, verifica se todos os que estão associados à escola possuem a habilitaçao requisitada pela mesma
                else if(assignments_professores[professor_avaliado] == "" && assignments_escolas[escola_escolhida].size() >= escolas_contratantes[escola_escolhida].back()){
                    trocar = true;
                    for(iter = assignments_escolas[escola_escolhida].begin(); iter != assignments_escolas[escola_escolhida].end() && assignments_escolas[escola_escolhida].size() > escolas_contratantes[escola_escolhida].back(); iter++){
                        //caso haja algum que não tenha a habilitação solicitada pela escola, o mesmo é retirado e o novo professor é colocado
                        if((*iter).back() != escolas_contratantes[escola_escolhida].front() && it->first.back() == escolas_contratantes[escola_escolhida].front() && trocar){
                            assignments_professores[*iter] = "";
                            assignments_escolas[escola_escolhida].erase(iter);
                            assignments_professores[professor_avaliado] = escola_escolhida;
                            assignments_escolas[escola_escolhida].push_front(professor_avaliado);
                            trocar = false;
                        }
                    }
                }
            }
        }

        cout << "Emparelhamento por professores: \n";
        for(print_assignment_prof = assignments_professores.begin(); print_assignment_prof != assignments_professores.end(); print_assignment_prof++){
            cout << "P" << print_assignment_prof->first.front() << ": ";
            cout << print_assignment_prof->second << endl;
        }

        cout << "\n\nEmparelhamento por escolas e suas respectivas vagas:\n";

        for(imprime_emparelhamento = escolas_contratantes.begin(); imprime_emparelhamento != escolas_contratantes.end(); imprime_emparelhamento++){
            cout << imprime_emparelhamento->first << ": ";

            for(get_professores = assignments_escolas[imprime_emparelhamento->first].begin(); get_professores != assignments_escolas[imprime_emparelhamento->first].end(); get_professores++){
                cout << "P" << (*get_professores).front() << "  ";
            }
            cout << "\n";
            
        }

    }
};

int main(){
    Graph gbipart;
    gbipart.inicializaMaps();
    gbipart.assignmentsArrangement();
}