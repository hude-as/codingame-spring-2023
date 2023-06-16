# g++ -std=gnu++17 -Werror -Wextra -O3 sprint2021.cpp -o bot2
# quom solutions/src/main.cpp solutions/out/maingen.cpp -s "~> stitch <~"
MERGER 			= quom
MERGER_OPTION 	= -s "~> stitch <~"
COMPILER		= g++ -std=gnu++17
FLAG			= -Wall -Werror -Wextra -O3
SAVE_BIN_DIR	= solutions/bin/save
NAME			= solutions/bin/bot
TEST			= solutions/bin/tests
INPUT_FILE		= solutions/tests/inputs/inputs.txt
SRC_MAIN		= solutions/src/main.cpp
SRC_TEST		= solutions/tests/main_testing.cpp
MAIN_GEN		= solutions/out/main_generated.cpp
MAIN_TEST_GEN	= solutions/out/main_testing_generated.cpp
MAIN_CPP		= main.cpp

# (step3) : increment exe number
# (step4) : version exe
# (step4) : launch test
# (step5) : launch brutal tester

# brutal tester bin1 bin2 jeu
# bin 1: win 60%
# bin 2 : win 40%

all: build $(TEST) $(NAME)

build:
	$(MERGER) $(SRC_MAIN) $(MAIN_GEN) $(MERGER_OPTION)
	$(MERGER) $(SRC_TEST) $(MAIN_TEST_GEN) $(MERGER_OPTION)

$(TEST):
	$(COMPILER)	$(FLAG) $(MAIN_TEST_GEN) -o $(TEST)

$(NAME):
	$(COMPILER)	$(FLAG) $(MAIN_GEN) -o $(NAME)

test:
	./$(TEST) < $(INPUT_FILE)

clean:
	rm -f $(NAME)
	rm -f $(TEST)

brutal:
	java -jar cg-brutaltester/target/cg-brutaltester-1.0.0-SNAPSHOT.jar -r "java -jar cg-brutaltester/SpringChallenge2023-brutaltester/target/spring-2023-ants-1.0-SNAPSHOT.jar" -p1 "$(P1EXEC)" -p2 "$(P2EXEC)" -t $(THREADS) -n $(GAMES) -l "./logs/" -s

quick:
	java -jar cg-brutaltester/target/cg-brutaltester-1.0.0-SNAPSHOT.jar -r "java -jar cg-brutaltester/SpringChallenge2023-brutaltester/target/spring-2023-ants-1.0-SNAPSHOT.jar" -p1 "$(P1EXEC)" -p2 "$(P2EXEC)" -t $(THREADS) -n $(QUICK_GAMES) -l "./logs/" -s

re: clean all

# OBJ			= $(SRCS:.c=.o)

# OBJECTS		= $(addprefix obj/, $(notdir $(OBJ)))
