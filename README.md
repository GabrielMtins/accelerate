# accelerate

# Objetivos

Eu já fiz vários jogos utilizando a linguagem C. Porém, sempre tive um
problema que não conseguia separar bem o código da engine do código do jogo.
Um problema de arquitetura, eu usava algo mais parecido com orientado à objeto,
inspirado no estilo de entidades da engine de Quake 2.

Como eu já estava reutilizando muitas vezes partes da engine, resolvi fazer uma
engine de fato duradoura. Ou seja, um dos meus objetivos é separar bem o que
é código de engine e código de jogo. Além disso, quero utilizar um paradigma
mais modular e reutilizável.

Para concretizar esse último objetivo, estarei finalmente utilizando um ECS ou
Entity-Component-System. Com esse sistema, irei fazer o módulo principal da engine,
com renderização 2d para textos, texturas, suporte à música e efeitos sonoros etc.
Assim, posso publicar módulos adicionais, como suporte à 3d básico, ou até módulos
para facilitar o desenvolvimento de alguns jogos, como desenvolvimento de jogos
topdown. Também pretendo lançar um módulo que dê suporte à scripting à engine,
mas isso somente num futuro distante. Já fiz uma engine com scripting em lua,
e foi realmente um problema implementar scripting logo no início do desenvolvimento.

Assim, a engine será distribuída em forma de biblioteca. Para criar cenas novas,
basta o programador extender cenas já implementadas, como a Scene2d, e adicionar
seus novos tipos de entidade. Quero fazer com que essa engine possa ser utilizada
nos meus futuros jogos.

Um dos meus objetivos também é aprender mais C++, eu sempre fui relutante com
essa linguagem por achar muito mais complexa que C, sendo C uma das minhas
linguagens favoritas.

Bem, é o que eu quero. Talvez esse repositório sirva como exemplo de como não
fazer uma engine, mas tentarei fazê-la da mesma forma.

# Objetivos mais imediatos
- Ajeitar o Makefile para transformar esse código em biblioteca.
- Escrever a documentação da engine.
- Fazer um jogo de exemplo.
- Implementar colisões 2d por quadtree.

# Objetivos à longo prazo
- Implementar um módulo de renderização 3d.
- Adicionar leitor de cenas em json ou lua.
- Adicionar scripting com lua.
