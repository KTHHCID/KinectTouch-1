NAME = kinect
SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

$(NAME) : $(OBJS)
	g++ -lfreenect -W -Wall -Wextra -lsfml-window -lsfml-graphics -lSDL -lGL -lGLU $(SRCS) -o $(NAME)

all : $(NAME)

clean :
	rm $(OBJS)
	rm *~

fclean	:	clean
		rm $(NAME)

re	:	fclean all