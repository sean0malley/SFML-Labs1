#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <random>

// Функция генерирования случайного целочисленного числа в указанных пределах.
// Диапазон чисел: [min, max]
int GetRandomNumber(int min, int max)
{
    // Получить случайное число - формула
    int num = min + rand() % (max - min + 1);
    // возвращаем случайное число
    return num;
}

// класс хранит прямоугольник и его данные
class Rectangle 
{
public:
    // конструктор, принимает координаты левого верхнего угла прямоугольника,
    // размер экрана и цвет
    Rectangle(sf::Vector2f position, sf::Vector2f screen, sf::Color rectColor)
    {
        // из вектора с размерами экрана получаем ширину экрана
        screen_width = screen.x;
        // из вектора с размерами экрана получаем высоту экрана
        screen_height = screen.y;
        // присваеваем цвет полю класса
        color = rectColor;
        // ширина прямоугольника, от 10 до 1/10 ширины окна
        size.x = (float)GetRandomNumber(10, int(screen_width / 10));
        // высота прямоугольника, от 10 до 1/10 высоты окна
        size.y = (float)GetRandomNumber(10, int(screen_height / 10));
        // скорость от 1 до 10
        speed = (float)GetRandomNumber(1, 10);
        // задаем размер прямоугольника
        rectangle.setSize(size);
        // задаем начальную позицию прямоугольника
        rectangle.setPosition(position);
    }

    // метод перемещает прямоугольник
    void Move()
    {
        // вычисляем насколько надо передвинуть прямоугольник по оси Х
        float movX = speed / 5 * dirX;
        // вычисляем насколько надо передвинуть прямоугольник по оси Y
        float movY = speed / 5 * dirY;

        // двигаем прямоугольник
        rectangle.move(movX, movY);

        // получаем новую позицию
        sf::Vector2f pos = rectangle.getPosition();

        // если достигли левой или правой стенки
        if (pos.x < 0 || pos.x + size.x > screen_width)
        {
            // меняем напрвлление движения по оси Х на противоположное
            dirX *= -1;
            // увеличиваем скорость на 1
            speed++;
        }

        // если достигли верхней или нижней стенки
        if (pos.y < 0 || pos.y + size.y > screen_height)
        {
            // меняем напрвлление движения по оси Y на противоположное
            dirY *= -1;
            // увеличиваем скорость на 1
            speed++;
        }
    }

    // метод отрисовывает прямоугольник, принимает аргумент, окно для отрисовки
    void Draw(sf::RenderWindow& window)
    {
        // закраштваем прямоугольник
        rectangle.setFillColor(color);
        // выводим прямоугольник на экран
        window.draw(rectangle);
    }

    // метод задания цвета, принимает аргумент цвет
    void setColor(sf::Color c) 
    {
        // присваеваем полю color цвет
        color = c;
    }

    // метод возвращает цвет прямоугольника
    sf::Color getColor() const
    {
        // возвращаем поле color
        return color;
    }

    // метод возвращает RectangleShape
    sf::RectangleShape getRect() const
    {
        // возвращаем поле rectangle
        return rectangle;
    }

    // метод вызывается при столкновении с другим прямоугольником
    // меняем направление движения на противоположное
    void Hit()
    {
        // умножаем значение поля dirX на -1
        dirX *= -1;
        // умножаем значение поля dirY на -1
        dirY *= -1;
    }

private:
    // поля класса
    // прямоугольная форма которая будет основой для отрисовки
    sf::RectangleShape rectangle;
    // высота и ширина окна
    float screen_width, screen_height;
    // цвет прямоугольника
    sf::Color color;
    // размер прямоугольника
    sf::Vector2f size;
    // направление движения по оси Х, 1 направо, -1 налево
    int dirX = 1;
    // направление движения по оси Y, 1 вниз, -1 вверх
    int dirY = 1;
    // скорость движения
    float speed;
};

// массив прямоугольников
std::vector<Rectangle> rects;

// функция начального создания прямоугольников
void init(int n, int width, int height)
{
    // массив цветов
    const sf::Color colors[] = { sf::Color::White, sf::Color::Red, sf::Color::Green,
        sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan };

    // в цикле создаем прямоугольники пока их не наберется нужное количество
    while (rects.size() < n)
    {
        // выбыраем случайный цвет из массива
        sf::Color c = colors[GetRandomNumber(0, 6)];
        // создаем прямоугольник, задаем случайные координаты,
        // учитываем, что задается точка левого верхнего угла и еще
        // нужно оставить место под ширину и высоту
        Rectangle rect(sf::Vector2f(GetRandomNumber(10, width - width / 10), 
            GetRandomNumber(10, height - height / 10)), 
            sf::Vector2f(width, height), c);

        // флаг для проверки пересечения нового прямоугольника с уже имеющимися
        bool inArray = false;

        // обходим все прямоугольники в массиве
        for (auto& rc : rects)
        {
            // получаем координаты углов первого прямоугольника
            sf::FloatRect f1 = rc.getRect().getGlobalBounds();
            // получаем координаты углов второго прямоугольника
            sf::FloatRect f2 = rect.getRect().getGlobalBounds();
            // проверяем пересечение
            bool collides = f1.intersects(f2);
            // если пересечение есть
            if (collides)
                // устанавливем флаг пересечения
                inArray = true;
        }
        // если пересечений нет
        if (!inArray)
            // добавляем прямоугольник в массив
            rects.push_back(rect);
    }
}


// функция создает окно и обрабатывает движение фигур
void Game(int width, int height,  int count, int render)
{
    // Установить генератор случайных чисел
    srand(time(NULL));
    // создаем окно
    sf::RenderWindow window(sf::VideoMode(width, height), "Rectangle movement");
    // счетчик циклов отрисовки
    int renderCount = 0;
    // включаем вертикальную синхронизацию,
    // частота обновлений окна равна частоте монитора
    window.setVerticalSyncEnabled(true);

    // создаем массив прямоугольников
    init(count, width, height);

    // главный цикл пока окно не закрыто
    while (window.isOpen())
    {
        // переменная для события
        sf::Event event;
        // пробуем получить событие из окна
        while (window.pollEvent(event))
        {
            // если окно пытаются звкрыть
            if (event.type == sf::Event::Closed)
                // закрываем окно
                window.close();
        }

        // увеличиваем счетчик рендеров
        renderCount++;

        // если достигли предела циклов рендера
        if (renderCount > render)
            // закрываем окно
            window.close();

        // обходим все прямоугольники в массиве
        for (auto& rc : rects) {
            // двигаем прямоугольник
            rc.Move();
        }


        // тут проверяем прямоугольники на пересечение друг с другом
        for (int i = 0; i < rects.size(); i++)
        {
            // второй цикл начинается не с 0, 
            // так как предыдущие прямоугольники уже были проверены на пересечение
            for (int j = i; j < rects.size(); j++)
            {
                // если прямоугольники разные
                if (i != j)
                {
                    // получаем координаты углов первого прямоугольника
                    sf::FloatRect f1 = rects[i].getRect().getGlobalBounds();
                    // получаем координаты углов второго прямоугольника
                    sf::FloatRect f2 = rects[j].getRect().getGlobalBounds();
                    // проверяем пересечение
                    bool collides = f1.intersects(f2);
                    // если пересечение есть
                    if (collides)
                    {
                        // получаем цвет прямоугольника 1
                        sf::Color c1 = rects[i].getColor();
                        // получаем цвет прямоугольника 2
                        sf::Color c2 = rects[j].getColor();
                        // устанавливаем прямоугольнику 1 цвет прямоугольника 2
                        rects[i].setColor(c2);
                        // устанавливаем прямоугольнику 2 цвет прямоугольника 1
                        rects[j].setColor(c1);
                        // вызываем метод столкновения для прямоугольника 1
                        rects[i].Hit();
                        // вызываем метод столкновения для прямоугольника 2
                        rects[j].Hit();
                    }
                }
            }
        }
        // очищаем окно
        window.clear();
        // обходим массив прямоугольников
        for (auto& rc : rects)
            // отрисовываем прямоугольник
            rc.Draw(window);
        // выводим изображение на экран
        window.display();
    }
}

// основная функция программы
int main()
{
    // функция для вывода русских букв в консоли
    setlocale(LC_ALL, "Russian");

    // переменные высота и ширина экрана, количество элементов и количество циклов отрисовки
    int width, height, count, render;

    // выводим строку
    std::cout << "Введите ширину окна (Не меньше 100):" << std::endl;
    // читаем введенное значение в переменную
    std::cin >> width;

    // выводим строку
    std::cout << "Введите высоту окна (Не меньше 100):" << std::endl;
    // читаем введенное значение в переменную
    std::cin >> height;

    // выводим строку
    std::cout << "Введите количество объектов (от 4 до 10):" << std::endl;
    // читаем введенное значение в переменную
    std::cin >> count;

    // выводим строку
    std::cout << "Введите количество циклов отрисовки (60 циклов 1 секунда):" << std::endl;
    // читаем введенное значение в переменную
    std::cin >> render;

    // если заданные значение правильные
    if (width >= 100 && height >= 100 && count >= 4 && count <= 10 && render > 0)
    {
        // показываем окно SFML
        Game(width, height, count, render);
    }
    else // если заданные значение неправильные
    {
        // выводим сообщения
        std::cout << "Неправильно введены данные!" << std::endl;
    }

    return 0;
}
