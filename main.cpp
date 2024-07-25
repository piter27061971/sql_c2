#include <iostream>
#include <pqxx/pqxx>
#include <pqxx/transaction>
#include <string>
#include <map>


class ClientManager {

public:

    ClientManager(const std::string& connection) : connection1{ connection } 
        
        //�����������, ������� ����������� ����������, ����� ���� � �����������
    {
        
        std::cout << "������� �����������" << std::endl;
    }

    void initDbStructure() // �����, ��������� ��������� �� (�������)
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

    int addClient(const std::string& firstName, const std::string& lastName, const std::string& email) //������ ������� � ���������� ��� clientId
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
                b1=a1.addClient("����","������","123@fr.ru");
                b2=a1.addClient("�������", "������", "nik@ya.ru");
                b3=a1.addClient("����������", "������", "piter@ya.ru");
                b4=a1.addClient("�������", "������", "svn@ya.ru");
                b5 = a1.addClient("�������", "������", "krt@ya.ru");
                b6 = a1.addClient("�����", "��������", "petrov@rambler.ru");
                b7 = a1.addClient("�������", "�������", "ftpnkl@yandex.ru");
                a1.updateClient(b6, "����", "����������", "tytytytytyt@yandex.ru");
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

                std::cout << "������ ��������� �������" << std::endl;
            }


        }
        catch (...)
        {
            std::cout << "�� ������� ������� �������." << std::endl;

        }
    }
    catch (pqxx::sql_error e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "�� ������� ����������� � ����." << std::endl;
    }
   

   



}


/*
�������� ��������� ��� ���������� ��������� �� C++.
����� ������� ������������ ���������� � ��������:
�	���,
�	�������,
�	email,
�	�������.
��������� � ���, ��� ������� � ������� ����� ���� �� ����, � ���, ��� � ���� ������. � ����� � �� ���� � ��������, ���� �� �� ������� ��� ���������.
��� ����� ����������� ��������� �� ��� �������� ���������� � �������� ����� �� �++ ��� ���������� ������� �� ���������� ��������:
1.	�����, ��������� ��������� �� (�������).
2.	�����, ����������� �������� ������ �������.
3.	�����, ����������� �������� ������� ��� ������������� �������.
4.	�����, ����������� �������� ������ � �������.
5.	�����, ����������� ������� ������� � ������������� �������.
6.	�����, ����������� ������� ������������� �������.
7.	�����, ����������� ����� ������� �� ��� ������ � �����, �������, email ��� ��������.
��� ������ �����������, �� ��� �� ������, ��� ������ ���� ������ ���. ����� ��������� �������������� ������ � ������.
����� ������������ ���, ��������������� ������ ����������� ���� ������.
���������
�� ������� ���� ������ �����, ����������� ������� ������ ������ �������������� :)
���������� ������
�	����� ������� �������, ����������� ����� transaction_base::exec, � � ���������� ����������� SQL ������ � "CREATE TABLE".
�	����� �������� ������� �������, ����������� ����� transaction_base::exec, � � ���������� ����������� SQL ������ � "INSERT".
�	����� �������� ������ � �������, ����������� ����� transaction_base::exec, � � ���������� ����������� SQL ������ � "UPDATE".
�	����� ������� ������ � ������� ��� �������, ����������� ����� transaction_base::exec, � � ���������� ����������� SQL ������ � "DELETE".
�	��� ������ ������� �� ��� ������ ����������� ����� transaction_base::query, � � ���������� ����������� SQL ������ � "SELECT".
*/