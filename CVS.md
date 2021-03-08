Grupo: 019

ISTID: ist193743



## Modo de Acesso

No texto que se segue:

- **###** designa o número de grupo com três dígitos, e.g., **012**
- **ISTID** designa o identificador de aluno, e.g. **ist123456**

Assim, os valores possíveis para a variável de ambiente **CVSROOT** (ou pela opção **-d** do comando "cvs") são:

-  ***/afs/ist.utl.pt/groups/leic-co/co21/t/cvs/###** (disponível em clientes AFS e utilizador com token válido para a célula ist.utl.pt)
- **:ext:ISTID@sigma.ist.utl.pt:\**/afs/ist.utl.pt/groups/leic-co/co21/t/cvs/###** (disponível via SSH; verificar que CVS_RSH=ssh)

## Cuidados a ter na Utilização do Repositório CVS

#### NÃO FAZER

- **cvs init** (não é necessário: o repositório já existe)
- **cvs import** (não é necessário: o projecto já existe)
- **cd** **/afs/ist.utl.pt/groups/leic-co/co21/t/cvs** (ou qualquer outra manipulação directa dos ficheiros do repositório: toda a interacção com o repositório deve ser limitada ao comando "cvs")

#### FAZER

Antes de tudo o mais, fazer cópias de segurança (vulgo "backups") de tudo o que já foi feito. 

O repositório já contém uma versão preliminar do projecto e devem ser  dados os seguintes passos, para continuar a desenvolver o projecto:

1. **cvs co fir** (permite criar localmente uma directoria controlada pelo CVS, com o nome **fir**, com a estrutura apresentada acima, contendo os ficheiros do projecto);
2. Editar/adicionar material do/ao projecto;
3. Caso sejam criados ficheiros ou directórios novos, fazer **cvs add ficheiro1 ficheiro2 ... ficheiro3** (adicionar cada ficheiro novo ao projecto no CVS);
4. **cvs update** (antes de enviar alteração locais para o repositório, verificar se há  actualizações a incorporar na cópia local e que não foi esquecido nenhum ficheiro importante: voltar a 3 até terem sido todos adicionados)
5. **cvs commit -m "Mensagem de commit."** (enviar alterações para o repositório)

#### Observações importantes

- No ponto 3 acima, é importante notar que quando se quer adicionar o  conteúdo de uma (sub)directoria, deve ser primeiro adicionada a própria  directoria e, só depois, o seu conteúdo.
- Não adicionar cópias do material de apoio (CDK e RTS) ao projecto (serão removidas e quaisquer  alterações perdidas -- além disso, lixo no repositório é penalizado).
- Pessoas sem área no AFS, ou sem login activado, não poderão aceder ao conteúdo  do repositório: para activar estes acessos, consultar o self-service do  CIIST (DSI). Em caso de dificuldade, consultar o corpo docente.
- A estrutura do projecto no CVS deve ser mantida como apresentado.
- Em caso de dúvidas, consultar o corpo docente.



Podem encontrar aqui uma breve guia sobre CVS: [CVS Crash Course](https://web.tecnico.ulisboa.pt/~david.matos/w/pt/index.php/CVS_Crash_Course).