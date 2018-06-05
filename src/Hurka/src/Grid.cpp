#include "Grid.hpp"



/// //////////////////////////////////////////////
/// Grid           cr 2018-02-22
// The grid follows the static grid size of 64 x 64 px


// (--)
Grid::Grid(int _height, int _width)
{
    width = _width;
    height = _height;
    texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRID_1.png");
    sprite = Sprite(texture);


    textureSelected.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRID_SELECTED.png");
    spriteSelected = Sprite(textureSelected);

    // Load font
    if (!font.loadFromFile("consola.ttf"))
    {
        std::cout << "ERROR " << cn << " could not load font.\n";
    }
}


// (--)
void Grid::draw( RenderTarget& rt, HPos *viewHPos)
{
    int gx = 0;
    int gy = 0;
    int wx = 0;
    int wy = 0;

    Vector2f wPos = Vector2f(); // SFML specific...


    /*Vector2f firstGPos = Vector2f();        // Game position of the first grid cell
    Vector2f firstWPos = Vector2f();        // Window position of the first grid cell

    bool once = false;

    */


    for(int M= 0; M<height; M++){
        for(int N= 0; N < width; N++) {

            gx = convert_iso_to_gpix_x(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT,0);
            gy = convert_iso_to_gpix_y(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT,0);

  /*          if(!once)
            {
                firstGPos.x = gx;
                firstGPos.y = gy;
            }
*/
            wx = gx + viewHPos->gpix_x;
            wy = gy + viewHPos->gpix_y;

/*            if(!once) {
                firstWPos.x = wx;
                firstWPos.y = wy;
                once = true;
            }
*/

            wPos.x = wx;
            wPos.y = wy;

            sprite.setPosition(wPos);
            rt.draw(sprite);

        }

    }


    // Draw the selected grid that should be more visible
    gx = convert_iso_to_gpix_x(selected_iso_pos.y, selected_iso_pos.x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);
    gy = convert_iso_to_gpix_y(selected_iso_pos.y, selected_iso_pos.x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);

    wx = gx+ viewHPos->gpix_x;
    wy = gy+ viewHPos->gpix_y;

    wPos.x = wx;
    wPos.y = wy;

    spriteSelected.setPosition(wPos);
    rt.draw(spriteSelected);



    /// Also draw coordinates in text for the top one
    /*
    text.setFont(font);

    std::stringstream sstm;
    sstm << "GPOS(" << firstGPos.y << ", " << firstGPos.x << ")\n";
    sstm << "WPOS(" << firstWPos.y << ", " << firstWPos.x << ")\n";

    std::string txtStr = sstm.str();
    text.setString(txtStr );
    text.setCharacterSize(12);
    text.setFillColor(sf::Color::White);


    // inside the main loop, between window.clear() and window.display()
    text.setPosition(firstWPos);
    rt.draw(text);
*/


}



// Hm... The Pos coming in have pix_positions..
//these pixel positions could have been generated by aa tall house block with height=64, while
//the grid is =32 so... we get a faulty gpix_x and gpix_y ... should really re-compute it if we need it here
// Beta material!

void Grid::setVisible(HPos *_pos)
{
    drawSelectedGrid = true;

    selected_iso_pos.y = _pos->abs_iso_y;
    selected_iso_pos.x = _pos->abs_iso_x;

    selected_pix_pos.y = _pos->gpix_y;
    selected_pix_pos.x = _pos->gpix_x;
}



// HPOSDELETE
/*
void Grid::setVisible(Vector2f iso_pos)
{
    drawSelectedGrid = true;


    selected_iso_pos = iso_pos;

    selected_pix_pos.x = convert_iso_to_gpix_x(iso_pos.y, iso_pos.x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);
    selected_pix_pos.y = convert_iso_to_gpix_y(iso_pos.y, iso_pos.x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0 );



 //   std::cout << "Visible grid position: " << selected_pix_pos.x << " , " << selected_pix_pos.y << "\n";
}

*/





void Grid::hideVisible()
{
    drawSelectedGrid = false;
}






//h�r var du och pseudokodade n�stan iv�g.. utan att ttesta findTile()



// CR7 - Work in progress 2018-06-02






// Uses internal findTile() function
HPos *Grid::findTile(HRect *entireGameBoard, HPos *searchPos, std::string ind)
{
    HRect *relRect = new HRect(entireGameBoard->absStart, entireGameBoard->rows, entireGameBoard->cols, entireGameBoard->heightPx, entireGameBoard->widthPx);
    return findTile(entireGameBoard, relRect, searchPos, ind);
}














/// \brief Given a searchpos with gpix values set, find an iso inside a hmatrix
/// \param entireGameBoard
/// \param relRect A smaller than entireGameBoard rectangle, used in a divide n conquer manner
/// \param searchPos Gpix values of search position
// RECURSIVE
// (--) Test
HPos *Grid::findTile(HRect *entireGameboard, HRect *relRect, HPos *searchPos, std::string ind)
{

    int debugLevel = 2;

    HPos *retpos = nullptr;


	if(debugLevel >=1) {
        std::cout << "\n\n";
        std::cout << ind << "findGrid---------------------------\n";
	}



    // TEST:
    std::cout << ind <<  "funkar insideXpixels?:\n";
    if( !relRect->insideXPixles( searchPos ) ) {
        return nullptr;
    }




    std::cout << ind <<  "funkar insideYpixels?:\n";
    if ( !relRect->insideYPixles( searchPos ) ) {
        return nullptr;
    }




    /// OK - it CAN be inside here, lets check if we need to do bruteforce or just "divide n conquer" again


	if(debugLevel >=2) {
        std::cout << ind << " - Ok, it CAN be inside this rectangle.\n\n";

        std::cout << ind << "- input entireGameboard:\n";
        entireGameboard->dump(ind);
        std::cout << "\n";


        std::cout << ind << "- input relrect:\n";
        relRect->dump(ind);
        std::cout << "\n";

        std::cout << ind << "- input searchPos:\n";
        searchPos->dump(ind + "   ");
        std::cout << "\n";
	}



	int nrTiles = relRect->nrTiles();
	std::cout << ind << "- nrTiles: " << nrTiles << "\n";



	if(nrTiles < 16) {
		return bruteForceFindTile( entireGameboard, relRect, searchPos, ind );                                // RECURSION END
                                                                 // Can I pass string like this? ind + " " ?
	}





	/// Approach it the smart way

	// Divide the submatrix into four squares


    int cols = entireGameboard->cols;       // FIXME Correct to use these values?
    int rows = entireGameboard->rows;



    int halfRows1 = -1;
    int halfRows2 = -1;
    int halfCols1 = -1;
    int halfCols2 = -1;



    // figure out how to divide the four squares, if rows n cols are not evenly divided by 2

    if(rows/2%2==0) {
        halfRows1 = rows/2;
        halfRows2 = halfRows1  -1;               // We need -1 because the last position is not 20 its 19, because 0->19 is 20 elements hope that makes sense.
    } else {
        halfRows1 = (rows-1)/2;
        halfRows2 = rows - halfRows1  -1;        // same here
    }


     if(rows/2%2==0) {
        halfCols1 = rows/2;
        halfCols2 = halfCols1   -1;              // same here
    } else {
        halfCols1 = (rows-1)/2;
        halfCols2 = rows - halfCols1    -1;      // same here
    }



    if(debugLevel >=2) {

        std::cout << "\n";
        std::cout << ind << "Divide the submatrix into four squares\n";

        std::cout << ind << " - halfRows1=" << halfRows1 << "\n";
        std::cout << ind << " - halfCols1=" << halfCols1 << "\n";
        std::cout << ind << " - halfRows2=" << halfRows2 << "\n";
        std::cout << ind << " - halfCols2=" << halfCols2 << "\n";


	}



	int sqHeight = -1;
	int sqWidth =-1;

	HRect *sq0, *sq1, *sq2, *sq3;






	// SQUARE 0



	int fromY= relRect->absStart->abs_iso_y;
	int fromX = relRect->absStart->abs_iso_x;
	int toY = fromY + halfRows1;
	int toX = fromX + halfCols1;

	sq0 = new HRect( fromY,         fromX,               toY,    toX, 32,64);







	fromY = fromY;         // Keep it as is
	fromX = toX +1;        // Continue from previous square, but add 1 so we dont overlap and run same tiles again
	toY = toY;             // Keep it as is
	toX = toX + halfCols2; // Add the other half

	sq1 = new HRect( fromY,         fromX,               toY,    toX, 32,64);





	// Beginning on a new line

	fromY = halfRows1 + 1;  // Continue from the previous squares, but add 1 so we dont overlap and run same tiles again
	fromX = relRect->absStart->abs_iso_x;
	toY = toY + halfRows2;                      // Add the other half
	toX = halfCols1;

    sq2 = new HRect( fromY,         fromX,               toY,    toX, 32,64);






	fromY = fromY;                   // Keep as it is
	fromX = fromX + halfCols1;
    toY = toY;                      // Keep as it is
    toX = toX + halfCols2;          // Add the other half


    sq3 = new HRect( fromY,         fromX,               toY,    toX, 32,64);





	if(debugLevel >=2) {

        std::cout << "\n";
        std::cout << ind << "Given entireGameBoard:\n";
        std::cout << ind << " - rows=" << rows << ", cols=" << cols << "\n";

        std::cout << ind << "we divided it up to these 4 squares:\n";

        sq0->dump("  ");
        sq1->dump("  ");
        sq2->dump("  ");
        sq3->dump("  ");

	}



	/// Enter each square and look for the grid

	retpos = findTile(entireGameboard, sq0, searchPos, ind ); // Does this indentation thing cock out the whole string thing?
	if(retpos != nullptr) { return retpos; }


    retpos = findTile(entireGameboard, sq1, searchPos, ind );
    if(retpos != nullptr) { return retpos; }



    retpos = findTile(entireGameboard, sq2, searchPos, ind );
    if(retpos != nullptr) { return retpos; }


    retpos = findTile(entireGameboard, sq3, searchPos, ind );
    if(retpos != nullptr) { return retpos; }








}


/// \brief internal function used by "findTile()" to go pixelline by line for every tile inside a relative rect
/// \param entireGameBoard
/// \param relRect A smaller than entireGameBoard rectangle, used in a divide n conquer manner
/// \param searchPos Gpix values of search position
// (--) Test
HPos *Grid::bruteForceFindTile(HRect *entireGameboard, HRect *relRect, HPos *searchPos, std::string ind)
{

    HPos *isopos = nullptr;


    // Generate iso pos for all the tiles inside the relrect
    // and check if we're inside it

    for(int Y = 0; Y < relRect->rows; Y++)
    {

        for(int X = 0; X < relRect->cols; X++)
         {


            // Current Tile:
            isopos    = new HPos(Y,X,USE_ISO);
            isopos->dump(" current tile: ");


            // Rough check if insideXpixel span

            if( isopos->gpix_x < searchPos->gpix_x  && searchPos->gpix_x < (isopos->gpix_x + GRID_TEXTURE_WIDTH) ) {
                std::cout << ind << "   - inside x span.\n";
            } else {
                return nullptr; // Next!
            }





            // Rough check if insideYpixel span


            if( isopos->gpix_y < searchPos->gpix_y  && searchPos->gpix_y < (isopos->gpix_y + GRID_TEXTURE_HEIGHT) ) {
                std::cout << ind << "   - inside y span.\n";
            } else {
                return nullptr; // Next!
            }







            ///
            /// Now compare if its inside here, use that nice... pixel by pixel row you did on paper
            ///





            //const int GRID_TEXTURE_HEIGHT = 32;
            //const int GRID_TEXTURE_WIDTH  = 64;




            //                      ##                  xwidth=2            xoffset=64/2 - width/2
            //                    ##$$##                xwidth=6    (+4)    xoffset=64/2 - width/2
            //                  ##$$##$$##              xwidth=10   (+4)
            //                ##$$##$$##$$##


            // Loop1 going downwards from a pointy tip


            int xwidth = 2;
            int xoffset = GRID_TEXTURE_WIDTH/2;     // =32 pixels
            xoffset += isopos->gpix_x;              // add the position of the Tile we're currently looking at

            int fromX  = -1;
            int toX = -1;

            std::cout << "\n\n";
            std::cout << ind << "Loop 1 begins:\n\n";

            std::cout << ind <<"              ##                  xwidth=2            xoffset=64/2 - width/2 \n"
                      << ind << "            ##$$##                xwidth=6    (+4)    xoffset=64/2 - width/2 \n"
                      << ind <<"          ##$$##$$##              xwidth=10   (+4) \n"
                      << ind <<"        ##$$##$$##$$## \n";



            for(int y = 0; y < (GRID_TEXTURE_HEIGHT/2); y++)
            {

                std::cout << ind << "xoffset=" << xoffset << "\n";
                std::cout << ind << "xwidth=" << xwidth << "\n";



                // Setup from and to

                fromX = xoffset;
                toX = xoffset + xwidth;

                if( fromX < searchPos->gpix_x  && searchPos->gpix_x < toX ) {
                    // inside!!
                    std::cout << ind << " - - - Eureka!  we are inside " << fromX << " - " << toX << "\n";
                    return isopos;
                }

                // No luck, please go down one pixelline and increase Xwidth and adjust xoffset
                xwidth+=4;
                xoffset = GRID_TEXTURE_WIDTH/2 - (int)floor(xwidth/2);

            }



            //
            //                ##$$##$$##$$##
            //                  ##$$##$$##
            //                    ##$$##
            //                      ##



            std::cout << "\n\n";
            std::cout << ind << "Loop 2 begins:\n\n";

            std::cout << ind << "        ##$$##$$##$$## \n"
                      << ind << "          ##$$##$$##   \n"
                      << ind << "            ##$$##     \n"
                      << ind << "              ##       \n";




            xwidth-=4;  // Just step back a bit from the last one,
            xoffset = GRID_TEXTURE_WIDTH/2 - (int)floor(xwidth/2); // and recalc this one...
            xoffset += isopos->gpix_x;  // add the position of the Tile we're currently looking at


            // Now we're good to go. Do the same thing in loop2 but reverse the operations
            // on xwidth and xoffset for every line:

            for(int y = 0; y < (GRID_TEXTURE_HEIGHT/2); y++)
            {

                std::cout << ind << "xoffset=" << xoffset << "\n";
                std::cout << ind << "xwidth=" << xwidth << "\n";

                // Setup from and to

                fromX = xoffset;
                toX = xoffset + xwidth;

                if( fromX < searchPos->gpix_x  && searchPos->gpix_x < toX ) {
                    // inside!!
                    std::cout << ind << " - - -  Eureka!  we are inside " << fromX << " - " << toX << "\n";
                    return isopos;
                }

                // No luck, please go down one pixelline and decrease Xwidth and adjust xoffset
                xwidth-=4;
                xoffset = GRID_TEXTURE_WIDTH/2 - (int)floor(xwidth/2);
            }



            std::cout << "------------STOP and look at result\n";
            return nullptr;
        }
    }



    std::cout << "NOT CODED YET\n";
    return nullptr;
}


