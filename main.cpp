#include <iostream>
#include <pqxx/pqxx>
#include <pqxx/transaction>
#include <string>
#include <map>


class ClientManager {

public:

    ClientManager(const std::string& connection) : connection1{ connection } 
        
        //конструктор, который настраивает соединение, может быть с параметрами
    {
        
        std::cout << "Удалось подключится" << std::endl;
    }

    void initDbStructure() // Метод, создающий структуру БД (таблицы)
    {
        std::string create1, create2;

        create1 = "CREATE TABLE if not exists person ( id serial NOT NULL, "
            " first_name VARCHAR(80) NOT NULL, "
            "last_name VARCHAR(80) NOT NULL, "
            " e_mail VARCHAR(80) unique NOT NULL , "
            " CONSTRAINT person_pk PRIMARY KEY(id) "
            " ); ";
        create2 = "CREATE TABLE if not exists telefon ("
            "id_person int not NULL,"
            " telefon VARCHAR(12) unique not NULL,"
            " CONSTRAINT telefon_person_fk FOREIGN KEY(id_person) REFERENCES person(id)"
            " ); ";

        pqxx::work tx1{ connection1 };
        tx1.exec(create1);
        tx1.exec(create2);
        tx1.commit();

    }

    int addClient(const std::string& firstName, const std::string& lastName, const std::string& email) //создаём клиента и возвращаем его clientId
    {
        int id1;
        pqxx::transaction tx3{ connection1 };
       
        tx3.exec("INSERT INTO person(first_name, last_name, e_mail) VALUES('" + tx3.esc(firstName) + "' , '" + tx3.esc(lastName) + "' , '" + tx3.esc(email) + "');");
        id1 = tx3.query_value<int>("select id from person where ((first_name='"
            "" + tx3.esc(firstName) + "') and (last_name ='"
            "" + tx3.esc(lastName) + "') and (e_mail ='"
            "" +tx3.esc( email) + "')); ");
        tx3.commit();
        return id1;
    }

    void addPhoneNumber(int clientId, const std::string& phoneNumber)
    {
        pqxx::work tx1{ connection1 };
        tx1.exec("INSERT INTO telefon (id_person, telefon) values ('" + tx1.esc(std::to_string(clientId)) + "','" + tx1.esc(phoneNumber) + "');");
        tx1.commit();

    }

    void updateClient(int clientId, const std::string& firstName, const std::string& lastName, const std::string& email)
    {
        pqxx::work tx1{ connection1 };
        tx1.exec("update person set first_name = '" + tx1.esc(firstName) + "', last_name ='" + tx1.esc(lastName) + "', e_mail = '" + tx1.esc(email) + "' "
            "where ((id='"
            "" + tx1.esc(std::to_string(clientId) )+ "')); ");
        tx1.commit();

    }

    void removeClient(int clientId)
    {
        pqxx::work tx1{ connection1 };
        tx1.exec("delete from telefon where (id_person='" + tx1.esc(std::to_string(clientId)) + "');");
        tx1.exec("delete from person where (id='" + tx1.esc(std::to_string(clientId)) + "');");
        tx1.commit();
    }


    void removetelefon(std::string telefon1)
    {     
            pqxx::work tx1{ connection1 };
            tx1.exec("delete from telefon where (telefon='" +tx1.esc( telefon1 )+ "');");
            tx1.commit();
    }


    int poisk(std::string first_name1, std::string last_name1, std::string e_mail1)
    {
        int id1;
            pqxx::work tx1{ connection1 };
            id1 = tx1.query_value<int>("select id from person where ((first_name='"
                "" +tx1.esc( first_name1) + "') and (last_name ='"
                "" + tx1.esc(last_name1) + "' and (e_mail ='"
                "" + tx1.esc(e_mail1)+ "')); ");
            tx1.commit();
            return id1;
    }


    int poisk(std::string telefon1)
    {
        int id1;
            pqxx::work tx1{ connection1 };
            id1 = tx1.query_value<int>("select id_person from telefon where (telefon='" + tx1.esc(telefon1) + "');");
            tx1.commit();
            return id1;
    }
private:

    pqxx::connection connection1;

};



int main()
{



    setlocale(LC_ALL, "Russian");

  
    std::string vvod="host=localhost "
        "port=5432 "
        "dbname=my_database "
        "user=postgres "
        "password=skipetr";
    try
    { 
        ClientManager a1(vvod);
        try
        {
            a1.initDbStructure();
            try
            {
                int b1, b2, b3, b4, b5, b6, b7;
                b1=a1.addClient("Петр","Иванов","123@fr.ru");
                b2=a1.addClient("Николай", "Иванов", "nik@ya.ru");
                b3=a1.addClient("Константин", "Петров", "piter@ya.ru");
                b4=a1.addClient("Аркадий", "Петров", "svn@ya.ru");
                b5 = a1.addClient("Аркадий", "Петров", "krt@ya.ru");
                b6 = a1.addClient("Семен", "Кузнецов", "petrov@rambler.ru");
                b7 = a1.addClient("Николай", "Семенов", "ftpnkl@yandex.ru");
                a1.updateClient(b6, "Олег", "Николаевич", "tytytytytyt@yandex.ru");
                a1.addPhoneNumber(b2, "3233188");
                a1.addPhoneNumber(b3, "3333333");
                a1.addPhoneNumber(b3, "7777777");
                a1.addPhoneNumber(b1, "5555555");
                a1.addPhoneNumber(b4, "5341269");
                a1.removeClient(b2);
                a1.removetelefon("7777777");
            }
            catch (...)
            {

                std::cout << "Ошибка обработки запроса" << std::endl;
            }


        }
        catch (...)
        {
            std::cout << "Не удалось создать таблицы." << std::endl;

        }
    }
    catch (pqxx::sql_error e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "Не удалось подключится к базе." << std::endl;
    }
   

   



}


/*
Создайте программу для управления клиентами на C++.
Нужно хранить персональную информацию о клиентах:
•	имя,
•	фамилия,
•	email,
•	телефон.
Сложность в том, что телефон у клиента может быть не один, а два, три и даже больше. А может и не быть — например, если он не захотел его оставлять.
Вам нужно разработать структуру БД для хранения информации и написать класс на С++ для управления данными со следующими методами:
1.	Метод, создающий структуру БД (таблицы).
2.	Метод, позволяющий добавить нового клиента.
3.	Метод, позволяющий добавить телефон для существующего клиента.
4.	Метод, позволяющий изменить данные о клиенте.
5.	Метод, позволяющий удалить телефон у существующего клиента.
6.	Метод, позволяющий удалить существующего клиента.
7.	Метод, позволяющий найти клиента по его данным — имени, фамилии, email или телефону.
Эти методы обязательны, но это не значит, что должны быть только они. Можно создавать дополнительные методы и классы.
Также предоставьте код, демонстрирующий работу написанного вами класса.
Подсказка
Не читайте этот раздел сразу, попытайтесь сначала решить задачу самостоятельно :)
Посмотреть методы
•	Чтобы создать таблицы, используйте метод transaction_base::exec, а в аргументах передавайте SQL запрос с "CREATE TABLE".
•	Чтобы добавить телефон клиента, используйте метод transaction_base::exec, а в аргументах передавайте SQL запрос с "INSERT".
•	Чтобы изменить данные о клиенте, используйте метод transaction_base::exec, а в аргументах передавайте SQL запрос с "UPDATE".
•	Чтобы удалить данные о клиенте или телефон, используйте метод transaction_base::exec, а в аргументах передавайте SQL запрос с "DELETE".
•	Для поиска клиента по его данным используйте метод transaction_base::query, а в аргументах передавайте SQL запрос с "SELECT".
*/