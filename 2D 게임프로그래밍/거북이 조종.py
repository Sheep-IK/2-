import turtle

turtle.shape("turtle")

def left():
    turtle.setheading(180)
    turtle.forward(50)
    turtle.stamp()

def up():
    turtle.setheading(90)
    turtle.forward(50)
    turtle.stamp()

def down():
    turtle.setheading(270)
    turtle.forward(50)
    turtle.stamp()

def right():
    turtle.setheading(0)
    turtle.forward(50)
    turtle.stamp()
    
turtle.onkey(left, 'a')
turtle.onkey(up, 'w')
turtle.onkey(down, 's')
turtle.onkey(right, 'd')
turtle.listen()

    

     
