from graphics import *
import random
import copy

############################################################
# BLOCK CLASS
############################################################

class Block(Rectangle):
    ''' Block class:
        Implement a block for a tetris piece
        Attributes: x - type: int
                    y - type: int
        specify the position on the tetris board
        in terms of the square grid
    '''

    BLOCK_SIZE = 30
    OUTLINE_WIDTH = 3

    def __init__(self, pos, color):
        self.x = pos.x
        self.y = pos.y
        
        p1 = Point(pos.x*Block.BLOCK_SIZE + Block.OUTLINE_WIDTH,
                   pos.y*Block.BLOCK_SIZE + Block.OUTLINE_WIDTH)
        p2 = Point(p1.x + Block.BLOCK_SIZE, p1.y + Block.BLOCK_SIZE)

        Rectangle.__init__(self, p1, p2)
        self.setWidth(Block.OUTLINE_WIDTH)
        self.setFill(color)

    def can_move(self, board, dx, dy):
        ''' Parameters: dx - type: int
                        dy - type: int

            Return value: type: bool
                        
            checks if the block can move dx squares in the x direction
            and dy squares in the y direction
            Returns True if it can, and False otherwise
            HINT: use the can_move method on the Board object
        '''
        # calls the can_move method in Board to check if the new position
        # (x+dx, y+dy) is available, i.e., if it is inside the board
        if board.can_move(self.x + dx, self.y + dy):
            return True
        else:
            return False
    
    def move(self, dx, dy):
        ''' Parameters: dx - type: int
                        dy - type: int
                        
            moves the block dx squares in the x direction
            and dy squares in the y direction
        '''

        self.x += dx
        self.y += dy

        Rectangle.move(self, dx*Block.BLOCK_SIZE, dy*Block.BLOCK_SIZE)

############################################################
# SHAPE CLASS
############################################################

class Shape():
    ''' Shape class:
        Base class for all the tetris shapes
        Attributes: blocks - type: list - the list of blocks making up the shape
                    rotation_dir - type: int - the current rotation direction of the shape
                    shift_rotation_dir - type: Boolean - whether or not the shape rotates
    '''

    def __init__(self, coords, color):
        self.blocks = []
        self.rotation_dir = 1
        ### A boolean to indicate if a shape shifts rotation direction or not.
        ### Defaults to false since only 3 shapes shift rotation directions (I, S and Z)
        self.shift_rotation_dir = False
        
        for pos in coords:
            self.blocks.append(Block(pos, color))



    def get_blocks(self):
        '''returns the list of blocks
        '''
        return self.blocks

    def draw(self, win):
        ''' Parameter: win - type: CanvasFrame

            Draws the shape:
            i.e. draws each block
        ''' 
        for block in self.blocks:
            block.draw(win)

    def undraw(self):
        ''' Parameter: win - type: CanvasFrame

            Undraws the shape:
            i.e. undraws each block
        '''
        for block in self.blocks:
            block.undraw()

    def move(self, dx, dy):
        ''' Parameters: dx - type: int
                        dy - type: int

            moves the shape dx squares in the x direction
            and dy squares in the y direction, i.e.
            moves each of the blocks
        '''
        for block in self.blocks:
            block.move(dx, dy)

    def can_move(self, board, dx, dy):
        ''' Parameters: dx - type: int
                        dy - type: int

            Return value: type: bool
                        
            checks if the shape can move dx squares in the x direction
            and dy squares in the y direction, i.e.
            check if each of the blocks can move
            Returns True if all of them can, and False otherwise
           
        '''
        # for each block in the shape, checks if it can move on the board
        for block in self.blocks:
            # if any block cannot move, then the shape also can't
            if not block.can_move(board, dx, dy):
                return False

        # if the algorithm got here, then all blocks can move, so can the shape
        return True
    
    def get_rotation_dir(self):
        ''' Return value: type: int
        
            returns the current rotation direction
        '''
        return self.rotation_dir

    def can_rotate(self, board):
        ''' Parameters: board - type: Board object
            Return value: type : bool
            
            Checks if the shape can be rotated.
            
            1. Get the rotation direction using the get_rotation_dir method
            2. Compute the position of each block after rotation and check if
            the new position is valid
            3. If any of the blocks cannot be moved to their new position,
            return False
                        
            otherwise all is good, return True
        '''
        # gets the rotation direction
        direction = self.get_rotation_dir()

        # gets the central block to serve as the pivot
        center = self.blocks[1]

        # for each other block in the block list (excluding the pivot)
        for block in self.blocks:
            if not block is center:
                # calculates the new position after rotation
                x = center.x - direction*center.y + direction*block.y
                y = center.y + direction*center.x - direction*block.x

                # if position not available, returns False
                if not board.can_move(x, y):
                    return False

        # if every block was able to rotate, return True
        return True

    def rotate(self, board):
        ''' Parameters: board - type: Board object

            rotates the shape:
            1. Get the rotation direction using the get_rotation_dir method
            2. Compute the position of each block after rotation
            3. Move the block to the new position
            
        '''    
        # gets the rotation direction
        direction = self.get_rotation_dir()

        # gets the central block to serve as the pivot
        center = self.blocks[1]

        # for each other block in the block list (excluding the pivot)
        for block in self.blocks:
            if not block is center:
                # calculates the new position after rotation
                x = center.x - direction*center.y + direction*block.y
                y = center.y + direction*center.x - direction*block.x

                # calls the move method on block, which expects displacements,
                # and not absolute positions (that's why we used differences)
                block.move(x - block.x, y - block.y)

        ### This should be at the END of your rotate code. 
        ### DO NOT touch it. Default behavior is that a piece will only shift
        ### rotation direciton after a successful rotation. This ensures that 
        ### pieces which switch rotations definitely remain within their 
        ### accepted rotation positions.
        if self.shift_rotation_dir:
            self.rotation_dir *= -1

        

############################################################
# ALL SHAPE CLASSES
############################################################

 
class I_shape(Shape):
    def __init__(self, center):
        coords = [Point(center.x - 2, center.y),
                  Point(center.x - 1, center.y),
                  Point(center.x    , center.y),
                  Point(center.x + 1, center.y)]
        Shape.__init__(self, coords, 'blue')
        self.shift_rotation_dir = True
        self.center_block = self.blocks[2]

class J_shape(Shape):
    def __init__(self, center):
        coords = [Point(center.x - 1, center.y),
                  Point(center.x    , center.y),
                  Point(center.x + 1, center.y),
                  Point(center.x + 1, center.y + 1)]
        Shape.__init__(self, coords, 'orange')        
        self.center_block = self.blocks[1]

class L_shape(Shape):
    def __init__(self, center):
        coords = [Point(center.x - 1, center.y),
                  Point(center.x    , center.y),
                  Point(center.x + 1, center.y),
                  Point(center.x - 1, center.y + 1)]
        Shape.__init__(self, coords, 'cyan')        
        self.center_block = self.blocks[1]


class O_shape(Shape):
    def __init__(self, center):
        coords = [Point(center.x    , center.y),
                  Point(center.x - 1, center.y),
                  Point(center.x   , center.y + 1),
                  Point(center.x - 1, center.y + 1)]
        Shape.__init__(self, coords, 'red')
        self.center_block = self.blocks[0]

    def rotate(self, board):
        # Override Shape's rotate method since O_Shape does not rotate
        return 

class S_shape(Shape):
    def __init__(self, center):
        coords = [Point(center.x    , center.y),
                  Point(center.x    , center.y + 1),
                  Point(center.x + 1, center.y),
                  Point(center.x - 1, center.y + 1)]
        Shape.__init__(self, coords, 'green')
        self.center_block = self.blocks[0]
        self.shift_rotation_dir = True
        self.rotation_dir = -1


class T_shape(Shape):
    def __init__(self, center):
        coords = [Point(center.x - 1, center.y),
                  Point(center.x    , center.y),
                  Point(center.x + 1, center.y),
                  Point(center.x    , center.y + 1)]
        Shape.__init__(self, coords, 'yellow')
        self.center_block = self.blocks[1]


class Z_shape(Shape):
    def __init__(self, center):
        coords = [Point(center.x - 1, center.y),
                  Point(center.x    , center.y), 
                  Point(center.x    , center.y + 1),
                  Point(center.x + 1, center.y + 1)]
        Shape.__init__(self, coords, 'magenta')
        self.center_block = self.blocks[1]
        self.shift_rotation_dir = True
        self.rotation_dir = -1      



############################################################
# BOARD CLASS
############################################################

class Board():
    ''' Board class: it represents the Tetris board

        Attributes: width - type:int - width of the board in squares
                    height - type:int - height of the board in squares
                    border - type:int - size of the board border in squares
                    canvas - type:CanvasFrame - where the pieces will be drawn
                    grid - type:Dictionary - keeps track of the current state of
                    the board; stores the blocks for a given position
                    status_msg - type: Text - stores the game status message
    '''
    
    def __init__(self, win, width, height, border):
        self.width = width
        self.height = height
        self.border = border

        # create a canvas to draw the tetris shapes on
        # a border has been added (twice on x, for both sides, and a single one
        # at y)
        self.canvas = CanvasFrame(win, (self.width+self.border*2) * Block.BLOCK_SIZE,
                                        (self.height+self.border) * Block.BLOCK_SIZE)
        self.canvas.setBackground('light gray')

        self.draw_border()

        # create an empty dictionary
        # currently we have no shapes on the board
        self.grid = {}

        # sets the status message at the center of the board
        message_pos = Point((self.width+self.border*2) * Block.BLOCK_SIZE * 0.5,
                            (self.height+self.border) * Block.BLOCK_SIZE * 0.5)

        # creates the text object with an empty message
        self.status_msg = Text(message_pos, "")

    def draw_border(self):
        for i in range(0, self.width+self.border*2):
            for j in range(0, self.height+self.border):
                if i < self.border or i >= self.width+self.border or \
                   j >= self.height:
                    block = Block(Point(i, j), 'dark gray')
                    block.setOutline('light gray')
                    block.draw(self.canvas)

    def draw_shape(self, shape):
        ''' Parameters: shape - type: Shape
            Return value: type: bool

            draws the shape on the board if there is space for it
            and returns True, otherwise it returns False
        '''
        if shape.can_move(self, 0, 0):
            shape.draw(self.canvas)
            return True
        return False

    def can_move(self, x, y):
        ''' Parameters: x - type:int
                        y - type:int
            Return value: type: bool

            1. check if it is ok to move to square x,y
            if the position is outside of the board boundaries, can't move there
            return False

            2. if there is already a block at that postion, can't move there
            return False

            3. otherwise return True
            
        '''
        # if x is outside the horizontal span of the board, or if y is beyond
        # the bottom limit (there is no need to test for the top limit), the
        # position is inaccessible (borders excluded)
        if x < self.border or x >= self.width+self.border or y >= self.height:
            return False
        # if the coordinate-pair (x, y) is in the grid, then this position is
        # already occupied by another block
        elif (x, y) in self.grid:
            return False
        else:
            return True

    def add_shape(self, shape):
        ''' Parameter: shape - type:Shape
            
            add a shape to the grid, i.e.
            add each block to the grid using its
            (x, y) coordinates as a dictionary key

            Hint: use the get_blocks method on Shape to
            get the list of blocks
            
        '''
        # gets the list of blocks from the shape
        blocks = shape.get_blocks()

        # adds each block to the grid, using the tuple (x, y), obtained from
        # its coordinates, as the key to the value
        for block in blocks:
            self.grid[(block.x, block.y)] = block


    def delete_row(self, y):
        ''' Parameters: y - type:int

            remove all the blocks in row y
            to remove a block you must remove it from the grid
            and erase it from the screen.
            If you dont remember how to erase a graphics object
            from the screen, take a look at the Graphics Library
            handout
            
        '''
        # gets all keys with second coordinate equals to y
        keys = [key for key in self.grid.keys() if y == key[1]]

        # gets each block in row y and removes it from the grid and the screen
        for key in keys:
            block = self.grid[key]
            block.undraw()
            del self.grid[key]
    
    def is_row_complete(self, y):        
        ''' Parameter: y - type: int
            Return value: type: bool

            for each block in row y
            check if there is a block in the grid (use the in operator) 
            if there is one square that is not occupied, return False
            otherwise return True
            
        '''
        # for each column in the row, checks if (x, y) is already added to the
        # grid, meaning there is a block there
        for column in range(self.border, self.width+self.border):
            # if there is still a free grid cell, then row is not complete
            if not self.grid.has_key((column, y)):
                return False

        # if all columns were in the grid, row is complete
        return True
    
    def move_down_rows(self, y_start):
        ''' Parameters: y_start - type:int                        

            for each row from y_start to the top
                for each column
                    check if there is a block in the grid
                    if there is, remove it from the grid
                    and move the block object down on the screen
                    and then place it back in the grid in the new position

        '''
        # traverses all rows back to the top, and all columns in each row
        for row in range(y_start, -1, -1):
            for column in range(self.border, self.width+self.border):
                # if coordinate pair (row, column) is in the grid
                if self.grid.has_key((column, row)):
                    # deletes the grid element, moves the block down 1 unit and
                    # reintroduces it in the grid with the new position as key
                    block = self.grid[(column, row)]
                    del self.grid[(column, row)]
                    block.move(0, 1)
                    self.grid[(column, row+1)] = block
                    
                    
    
    def remove_complete_rows(self):
        ''' Return value - type:int

            Removes all the complete rows and returns the number of deleted rows
            1. for each row, y, 
            2. check if the row is complete
                if it is,
                    delete the row
                    move all rows down starting at row y - 1

        '''
        # traverses all rows in the board and calls the appropriate methods
        deleted = 0
        for row in range(0, self.height):
            if self.is_row_complete(row):
                self.delete_row(row)
                self.move_down_rows(row-1)
                deleted += 1
        return deleted

    def game_over(self):
        ''' display "Game Over !!!" message in the center of the board
            HINT: use the Text class from the graphics library
        '''
        # sets the Game Over message and draws it
        self.status_msg.setText("Game Over !!!")
        self.status_msg.draw(self.canvas)

    def pause(self, paused):
        ''' Parameters: paused - type:bool

            display "PAUSE" message in the center of the board if parameter is
            True; otherwise, undraws the message
        '''
        # if paused, sets the pause message and draws it
        if paused:
            self.status_msg.setText("PAUSE")
            self.status_msg.draw(self.canvas)
        # if game was unpaused, undraws the message
        else:
            self.status_msg.undraw()


############################################################
# PIECE PREVIEW CLASS
############################################################

class PiecePreview():
    ''' PiecePreview class: controls next piece exhibition

        Attributes:
            width - type:int - the width of the piece preview frame
            height - type:int - the height of the piece preview frame
            canvas - type: CanvasFrame - frame where the preview is drawn
            shape - type: Shape - shape being previewed
    '''
    def __init__(self, win, width, height, border):
        # initializes class attributes
        self.width = width + border*2
        self.height = height + border

        # creates the piece preview frame with width and height passed as
        # parameters (keeping the BLOCK_SIZE factor) and a darker gray level
        self.canvas = CanvasFrame(win, self.width * Block.BLOCK_SIZE,
                                       self.height * Block.BLOCK_SIZE)
        self.canvas.setBackground('dark gray')

        self.shape = None

    def update_shape(self, shape):
        ''' Parameters: shape - type: Shape

            Just updates the preview shape with the one passed as parameter
        '''
        # just a safety check before undrawing, although it shouldn't happen
        if self.shape:
            # undraws the last shape
            self.shape.undraw()

        # updates the new next shape with a deep copy of the original object,
        # otherwise it can't be drawn (it would be already drawn in another
        # frame
        self.shape = copy.deepcopy(shape)

        # moves it 1 unit down (plus 1 down and 2 right to compensate for the
        # border) just to better position it on the frame
        self.shape.move(2, 2)

        # draws the copied shape
        self.shape.draw(self.canvas)

############################################################
# SCORE BOARD CLASS
############################################################

class ScoreBoard():
    ''' ScoreBoard class: controls Tetris scoring

        Attributes:
            BASE_SCORE - type:int - the base score used in score calculations
            width - type:int - the width of the score board frame
            height - type:int - the height of the score board frame
            canvas - type: CanvasFrame - frame where the score is drawn
            score - type:int - the accumulated player score
            level - type:int - current player level
    '''
    
    BASE_SCORE = 10
    
    def __init__(self, win, width, height, border):
        # initializes class attributes
        self.width = width + border*2
        self.height = height + border

        # creates the score board frame with width and height passed as
        # parameters (keeping the BLOCK_SIZE factor) and a darker gray level
        self.canvas = CanvasFrame(win, self.width * Block.BLOCK_SIZE,
                                       self.height * Block.BLOCK_SIZE)
        self.canvas.setBackground('dark gray')

        # score is initially zero
        self.score = 0

        # creates, sets and draws the text object for the score at the center of
        # the board
        text_score_pos = Point(self.width * Block.BLOCK_SIZE * 0.5,
                            self.height * Block.BLOCK_SIZE * 0.3)
        self.text_score = Text(text_score_pos, "Score: " + str(self.score))
        self.text_score.draw(self.canvas)

        # sets initial level to 1 and creates a text object to show it
        self.level = 1
        text_level_pos = Point(self.width * Block.BLOCK_SIZE * 0.5,
                               self.height * Block.BLOCK_SIZE * 0.7)
        self.text_level = Text(text_level_pos, "Level: " + str(self.level))
        self.text_level.draw(self.canvas)

    def get_level(self):
        ''' Return value - type:int

            Returns the current player level
        '''
        return self.level

    def add_score(self, nrows):
        ''' Parameters: nrows - type:int
            Adds the row-based score to the accumulated player score stored
            in the class attribute
        '''
        # adds the base score to the accumulated score, multiplying it by a
        # factor when the number of deleted rows is bigger than one (starting
        # from 4 deleted rows, player gets a huge bonus (10x))
        if nrows >= 4:
            self.score += self.BASE_SCORE * 10
        elif nrows > 1:
            self.score += self.BASE_SCORE * nrows * 1.5
        else:
            self.score += self.BASE_SCORE

        # updates graphic text object
        self.text_score.setText("Score: " + str(self.score))

        # every time user reaches 100 points, level increases
        if self.score > (self.level * 100):
            self.level += 1
            self.text_level.setText("Level: " + str(self.level))

############################################################
# TETRIS CLASS
############################################################

class Tetris():
    ''' Tetris class: Controls the game play
        Attributes:
            SHAPES - type: list (list of Shape classes)
            DIRECTION - type: dictionary - converts string direction to (dx, dy)
            BOARD_WIDTH - type:int - the width of the board
            BOARD_HEIGHT - type:int - the height of the board
            INITIAL_DELAY - type:int - the initial speed in milliseconds
            board - type:Board - the tetris board
            win - type:Window - the window for the tetris game
            delay - type:int - the current speed for moving the shapes
            current_shapes - type: Shape - the current moving shape on the board
            next_shape - type: Shape - the next shape on the queue
            paused - type: bool - current game status (paused or unpaused)
    '''
    
    SHAPES = [I_shape, J_shape, L_shape, O_shape, S_shape, T_shape, Z_shape]
    DIRECTION = {'Left':(-1, 0), 'Right':(1, 0), 'Down':(0, 1)}
    BOARD_WIDTH = 10
    BOARD_HEIGHT = 20
    BOARD_BORDER = 2
    INITIAL_DELAY = 1000
    
    def __init__(self, win):
        # creates the preview, the main and the score boards
        self.preview_board = PiecePreview(win, self.BOARD_WIDTH,
                                               self.BOARD_HEIGHT*0.2,
                                               self.BOARD_BORDER)
        self.board = Board(win, self.BOARD_WIDTH, self.BOARD_HEIGHT,
                                self.BOARD_BORDER)
        self.score_board = ScoreBoard(win, self.BOARD_WIDTH,
                                           self.BOARD_HEIGHT*0.1,
                                           self.BOARD_BORDER)
        self.win = win
        self.delay = self.INITIAL_DELAY #ms

        # sets up the keyboard events
        # when a key is called the method key_pressed will be called
        self.win.bind_all('<Key>', self.key_pressed)

        # set the current and next shapes to a random new shape
        self.current_shape = self.create_new_shape()
        self.next_shape = self.create_new_shape()

        # Draw the current_shape oan the board (take a look at the
        # draw_shape method in the Board class)
        self.board.draw_shape(self.current_shape)

        # Previews the next shape
        self.preview_board.update_shape(self.next_shape)

        # game is initially unpaused
        self.paused = False

        # For Step 9:  animate the shape!
        self.animate_shape()


    def create_new_shape(self):
        ''' Return value: type: Shape
            
            Create a random new shape that is centered
             at y = 0 and x = int(self.BOARD_WIDTH/2)
            return the shape
        '''
        # finds a random shape
        index = random.randint(0, len(self.SHAPES)-1);

        # initializes a Point object with initial shape position (top center)
        center = Point(int(self.BOARD_WIDTH/2), 0)

        # instantiates and returns the new shape
        shape = self.SHAPES[index](center)
        return shape
    
    def animate_shape(self):
        ''' animate the shape - move down at equal intervals
            specified by the delay attribute
        '''
        
        self.do_move('Down')
        self.win.after(self.delay, self.animate_shape)
    
    def do_move(self, direction):
        ''' Parameters: direction - type: string
            Return value: type: bool

            Move the current shape in the direction specified by the parameter:
            First check if the shape can move. If it can, move it and return True
            Otherwise if the direction we tried to move was 'Down',
            1. add the current shape to the board
            2. remove the completed rows if any 
            3. create a new random shape and set current_shape attribute
            4. If the shape cannot be drawn on the board, display a
               game over message

            return False

        '''
        # pieces only move if game is unpaused
        if not self.paused:
            # gets the values in x and y for movement
            dx = self.DIRECTION[direction][0]
            dy = self.DIRECTION[direction][1]

            # if the shape can move on the board, moves it and returns True
            if self.current_shape.can_move(self.board, dx, dy):
                self.current_shape.move(dx, dy)
                return True
            # if it can't move and it tried to move down
            elif direction == 'Down':
                # then the shape is added to the grid and new shape is created
                # at the top of the board (next shape for preview is updated)
                self.board.add_shape(self.current_shape)
                self.current_shape = self.next_shape
                self.next_shape = self.create_new_shape()
                if not self.board.draw_shape(self.current_shape):
                    self.board.game_over()

                self.preview_board.update_shape(self.next_shape)

                # if any row was completed, remove it
                nrows = self.board.remove_complete_rows()

                # passes the number of deleted rows to the score board for score
                # calculation, if any row was deleted
                if nrows > 0:
                    self.score_board.add_score(nrows)

                    # adjusts speed based on player level
                    self.delay = self.INITIAL_DELAY / self.score_board.get_level()

            # if control flow reached here, then movement was not possible
            return False

    def do_rotate(self):
        ''' Checks if the current_shape can be rotated and
            rotates if it can
        '''
        # pieces only rotate if game is unpaused
        if not self.paused:
            # just calls the appropriate rotation methods from Shape
            if self.current_shape.can_rotate(self.board):
                self.current_shape.rotate(self.board)
    
    def key_pressed(self, event):
        ''' this function is called when a key is pressed on the keyboard
            it currenly just prints the value of the key

            Modify the function so that if the user presses the arrow keys
            'Left', 'Right' or 'Down', the current_shape will move in
            the appropriate direction

            if the user presses the space bar 'space', the shape will move
            down until it can no longer move and is added to the board

            if the user presses the 'Up' arrow key ,
                the shape should rotate.

        '''

        # grabs the pressed key
        key = event.keysym

        # if the key is a valid direction, calls movement function
        if key in self.DIRECTION.keys():
            self.do_move(key)
            
        # if key pressed is the up arrow, then calls rotation function
        elif key == 'Up':
            self.do_rotate()
            
        # if key pressed is the space key
        elif key == 'space':
            # moves the current shape down until it can't move anymore
            while self.do_move('Down'):
                pass

        # if key pressed is the letter P
        elif key == 'p' or key == 'P':
            self.paused = not self.paused
            self.board.pause(self.paused)
            
       
################################################################
# Start the game
################################################################

win = Window("Tetris")
game = Tetris(win)
win.mainloop()
