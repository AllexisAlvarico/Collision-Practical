#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <GameObject.h>
#include <Player.h>
#include <NPC.h>
#include <Input.h>
#include <Debug.h>

using namespace std;

enum playerShape
{
	square,
	circle,
	ray
};




int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
	c2AABB test;
	test.min = c2V(-40.0f, -40.0f);
	test.max = c2V(-15.0f, -15.0f);
	// Load a NPC's sprites to display
	sf::Texture npc_texture;
	if (!npc_texture.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Load a mouse texture to display
	sf::Texture player_texture;
	if (!player_texture.loadFromFile("assets\\player.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	playerShape m_currentState = square;

	// Setup NPC's Default Animated Sprite
	AnimatedSprite npc_animated_sprite(npc_texture);
	npc_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup Players Default Animated Sprite
	AnimatedSprite player_animated_sprite(player_texture);
	player_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));


	//player circle
	sf::CircleShape playerCircle;
	playerCircle.setRadius(30);
	playerCircle.setOrigin(sf::Vector2f(30, 30));
	playerCircle.setFillColor(sf::Color::White);
	playerCircle.setOutlineThickness(2);
	//playerCircle.setOutlineColor(sf::Color::White);


	// Setup the NPC
	GameObject& npc = NPC(npc_animated_sprite);

	// Setup the Player
	GameObject& player = Player(player_animated_sprite);

	//Setup NPC AABB
	c2AABB aabb_npc;
	aabb_npc.min = c2V(npc.getAnimatedSprite().getPosition().x, npc.getAnimatedSprite().getPosition().y);
	aabb_npc.max = c2V(
		npc.getAnimatedSprite().getPosition().x +
		npc.getAnimatedSprite().getGlobalBounds().width,
		npc.getAnimatedSprite().getPosition().y +
		npc.getAnimatedSprite().getGlobalBounds().height);

	//Setup Player AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(player.getAnimatedSprite().getPosition().x, player.getAnimatedSprite().getPosition().y);
	aabb_player.max = c2V(player.getAnimatedSprite().getGlobalBounds().width / 6, player.getAnimatedSprite().getGlobalBounds().width / 6);

	//Setup for the capsule
	c2Capsule capsule_npc;
	capsule_npc.a = c2V(200, 200);
	capsule_npc.b = c2V(250, 200);
	capsule_npc.r = 20;



	c2Poly poly_npc;
	poly_npc.count = 5;
	poly_npc.verts[0] = { 400, 400 };
	poly_npc.verts[1] = { 450, 440 };
	poly_npc.verts[2] = { 420, 520 };
	poly_npc.verts[3] = { 330, 400 };
	poly_npc.verts[4] = { 400, 400 };


	c2Circle circle_npc;
	circle_npc.p.x = 400;
	circle_npc.p.y = 250;
	circle_npc.r = 30;

	sf::CircleShape npcCircle;
	npcCircle.setPosition(sf::Vector2f(400, 250));
	npcCircle.setRadius(30);
	npcCircle.setOrigin(sf::Vector2f(30, 30));
	npcCircle.setFillColor(sf::Color::Cyan);

	c2Circle circle_player;
	circle_player.p = c2V(playerCircle.getPosition().x , playerCircle.getPosition().y);
	circle_player.r = 30;


	sf::Vector2f pA = { 500,400 };
	sf::Vector2f pB = { 600,400 };
	sf::Vector2f dv = pB - pA;
	float magnitude = sqrt(dv.x * dv.x + dv.y * dv.y);
	sf::Vector2f unit = { dv / magnitude };

	c2Ray ray_npc;
	ray_npc.p = { pA.x, pA.y };
	ray_npc.t = magnitude;
	ray_npc.d = { unit.x, unit.y };

	//player ray cast
	sf::Vector2f lineA = { 500,400 };
	sf::Vector2f lineB = { sf::Vector2f((sf::Mouse::getPosition(window).x),(sf::Mouse::getPosition(window).y)) };
	sf::Vector2f playerdv = lineB - lineA;
	float pMagnitude = sqrt(playerdv.x * playerdv.x + playerdv.y * playerdv.y);
	sf::Vector2f pUnit = { playerdv / pMagnitude };

	c2Ray ray_player;
	ray_player.p = { lineA.x , lineA.y };
	ray_player.t = pMagnitude;
	ray_player.d = { pUnit.x,pUnit.y };

	c2Raycast ray_cast;


	// Shapes for the collisions
	// box to capsule
	sf::CircleShape capsule1;
	capsule1.setRadius(20);
	capsule1.setOrigin(sf::Vector2f(20,20));
	capsule1.setPosition(200, 200);
	capsule1.setFillColor(sf::Color::Yellow);
	sf::CircleShape capsule2;
	capsule2.setRadius(20);
	capsule2.setOrigin(sf::Vector2f(20, 20));
	capsule2.setPosition(250, 200);
	capsule2.setFillColor(sf::Color::Yellow);
	sf::RectangleShape capsuleBox;
	capsuleBox.setPosition(200, 180);
	capsuleBox.setSize(sf::Vector2f(50,40));
	capsuleBox.setFillColor(sf::Color::Yellow);
	// box to polygon
	sf::ConvexShape polygon;
	polygon.setPointCount(5);
	polygon.setPoint(0, sf::Vector2f(400, 400));
	polygon.setPoint(1, sf::Vector2f(450, 440));
	polygon.setPoint(2, sf::Vector2f(420, 520));
	polygon.setPoint(3, sf::Vector2f(330, 400));
	polygon.setPoint(4, sf::Vector2f(400, 400));
	polygon.setFillColor(sf::Color::Red);
	//box to ray
	sf::Vertex ray_line[]
	{

		sf::Vertex(pA),
		sf::Vertex(pB)

	};

	sf::Vertex ray_pLine[]
	{

		sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window))),
		sf::Vertex(lineB + sf::Vector2f(sf::Mouse::getPosition(window)))

	};






	// Initialize Input
	Input input;

	// Collision result
	int result = 0;

	// Direction of movement of NPC
	sf::Vector2f direction(0.1f, 0.2f);
	
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		if (m_currentState == square)
		{
			player.getAnimatedSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		}
		else if (m_currentState == circle)
		{
			playerCircle.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
		}
		
		// Move The NPC
		sf::Vector2f move_to(npc.getAnimatedSprite().getPosition().x + direction.x, npc.getAnimatedSprite().getPosition().y + direction.y);

		if (move_to.x < 0) {
			direction.x *= -1;
			move_to.x = 0 + npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.x + npc.getAnimatedSprite().getGlobalBounds().width >= 800) { 
			direction.x *= -1;
			move_to.x = 800 - npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.y < 0) { 
			direction.y *= -1;
			move_to.y = 0 + npc.getAnimatedSprite().getGlobalBounds().height;
		}
		else if (move_to.y + npc.getAnimatedSprite().getGlobalBounds().height >= 600) {
			direction.y *= -1;
			move_to.y = 600 - npc.getAnimatedSprite().getGlobalBounds().height;
		}
		
		npc.getAnimatedSprite().setPosition(move_to);

		// Update NPC AABB set x and y
		aabb_npc.min = c2V(
			npc.getAnimatedSprite().getPosition().x, 
			npc.getAnimatedSprite().getPosition().y
		);

		aabb_npc.max = c2V(
			npc.getAnimatedSprite().getPosition().x +
			npc.getAnimatedSprite().getGlobalBounds().width,
			npc.getAnimatedSprite().getPosition().y +
			npc.getAnimatedSprite().getGlobalBounds().height
		);

		// Update Player AABB
		aabb_player.min = c2V(
			player.getAnimatedSprite().getPosition().x, 
			player.getAnimatedSprite().getPosition().y
		);
		aabb_player.max = c2V(
			player.getAnimatedSprite().getPosition().x +
			player.getAnimatedSprite().getGlobalBounds().width,  
			player.getAnimatedSprite().getPosition().y + 
			player.getAnimatedSprite().getGlobalBounds().height
		);



		sf::VertexArray sqaure{ sf::LinesStrip,5 };
		// sets up the array for the square
		if (m_currentState == square)
		{
			sqaure[0].position = sf::Vector2f(sf::Mouse::getPosition(window)); // top left
			sqaure[1].position = sf::Vector2f(sf::Mouse::getPosition(window).x + player.getAnimatedSprite().getGlobalBounds().width, sf::Mouse::getPosition(window).y); // top right
			sqaure[2].position = sf::Vector2f(sf::Mouse::getPosition(window).x + player.getAnimatedSprite().getGlobalBounds().width, sf::Mouse::getPosition(window).y + player.getAnimatedSprite().getGlobalBounds().height); // bottom right
			sqaure[3].position = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y + player.getAnimatedSprite().getGlobalBounds().height); // bottom left
			sqaure[4].position = sf::Vector2f(sf::Mouse::getPosition(window)); // top left
		}


		if (m_currentState == circle)
		{
			playerCircle.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
			circle_player.p.x = playerCircle.getPosition().x;
			circle_player.p.y = playerCircle.getPosition().y;
		}

		if (m_currentState == ray)
		{
			ray_pLine[0].position = sf::Vector2f(sf::Mouse::getPosition(window)) + sf::Vector2f(lineA.x, 0);
			ray_player.p.x = (sf::Mouse::getPosition(window).x);
			ray_player.p.y = (sf::Mouse::getPosition(window).y);
			ray_pLine[1].position = sf::Vector2f(sf::Mouse::getPosition(window));
		
		}



		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					m_currentState = square;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					m_currentState = circle;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					m_currentState = ray;
				}
				break;
			default:
				input.setCurrent(Input::Action::IDLE);
				break;
			}
		}

		// Handle input to Player
		player.handleInput(input);

		// Update the Player
		player.update();

		// Update the Player
		npc.update();

		// Check for collisions

		/// <summary>
		/// Square collision
		/// </summary>
		/// <returns></returns>
		if (m_currentState == square)
		{
			if (c2AABBtoAABB(aabb_player, aabb_npc))
			{
				player.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
				for (int i = 0; i < 5; i++)
				{
					sqaure[i].color = sf::Color::Red;
				}
			}
			else if (c2AABBtoCapsule(aabb_player, capsule_npc))
			{
				for (int i = 0; i < 5; i++)
				{
					sqaure[i].color = sf::Color::Red;
				}
			}
			else if (c2AABBtoPoly(aabb_player, &poly_npc, NULL))
			{
				for (int i = 0; i < 5; i++)
				{
					sqaure[i].color = sf::Color::Red;
				}
			}
			else if (c2RaytoAABB(ray_npc, aabb_player, &ray_cast))
			{
				for (int i = 0; i < 5; i++)
				{
					sqaure[i].color = sf::Color::Red;
				}
			}
			else
			{
				for (int i = 0; i < 5; i++)
				{
					sqaure[i].color = sf::Color::White;
				}
			}


		}
		
		/// <summary>
		/// The circle collision
		/// </summary>
		/// <returns></returns>
		if (m_currentState == circle)
		{
			if (c2CircletoAABB(circle_player, aabb_npc))
			{
				//cout << "it has collided\n";
				playerCircle.setOutlineColor(sf::Color::Red);
			}
			else if (c2CircletoCircle(circle_player, circle_npc))
			{
				playerCircle.setOutlineColor(sf::Color::Red);
			}
			else if (c2RaytoCircle(ray_npc, circle_player, &ray_cast))
			{
				playerCircle.setOutlineColor(sf::Color::Red);
			}
			else if (c2CircletoCapsule(circle_player, capsule_npc))
			{
				playerCircle.setOutlineColor(sf::Color::Red);
			}
			else if (c2CircletoPoly(circle_player, &poly_npc, NULL))
			{
				playerCircle.setOutlineColor(sf::Color::Red);
			}
			else
			{
				playerCircle.setOutlineColor(sf::Color::White);
			}

		}

		if (m_currentState == ray)
		{

		}


		// Clear screen
		window.clear();


		if (m_currentState == square)
		{
			// Draw the Players Current Animated Sprite
			window.draw(player.getAnimatedSprite());
		}

		if (m_currentState == circle)
		{
			window.draw(playerCircle);
		}

		if (m_currentState == ray)
		{
			window.draw(ray_pLine, 2, sf::Lines);
		}
		window.draw(sqaure);
		window.draw(capsule1);
		window.draw(capsule2);
		window.draw(capsuleBox);
		window.draw(polygon);
		window.draw(npcCircle);
		window.draw(ray_line, 2, sf::Lines);
	
		// Draw the NPC's Current Animated Sprite
		window.draw(npc.getAnimatedSprite());

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};