NAME = pipex1
NAME_BONUS = pipex

SRC = pipex.c utils.c utils1.c\
		
SRC_BONUS = pipex_bonus.c utils.c utils1.c get_next_line.c get_next_line_utils.c\
			here_doc.c\


OBJ =   pipex.o utils.o utils1.o\

OBJ_BONUS = pipex_bonus.o utils.o utils1.o get_next_line.o get_next_line_utils.o\
		here_doc.o\

all : $(NAME)

$(NAME): $(SRC)
	
	@gcc -Wall -Wextra -Werror  -g $(SRC) -o $(NAME)
	@echo "👍👍👍"

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(SRC_BONUS)
	@gcc -Wall -Wextra -Werror -g $(SRC_BONUS) -o $(NAME_BONUS)
	@echo "🎁🎁🎁"


clean :
 
	@rm -f $(OBJ) $(OBJ_BONUS)
	@echo "🚮🚮🚮"

fclean : clean
	@rm -f $(NAME) $(NAME_BONUS)
	@echo "🗑️ 🗑️ 🗑️"

re : fclean all