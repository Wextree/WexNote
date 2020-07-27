package domain;

public class Student extends People{
    private String name;
    private Integer age;
    private Boolean gender;
    private Double money;

    public Student() {
    }

    public Student(String name, Integer age, Boolean gender, Double money) {
        this.name = name;
        this.age = age;
        this.gender = gender;
        this.money = money;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    public Boolean getGender() {
        return gender;
    }

    public void setGender(Boolean gender) {
        this.gender = gender;
    }

    public Double getMoney() {
        return money;
    }

    public void setMoney(Double money) {
        this.money = money;
    }

    public void sayHello(){
        System.out.println("hello!");
    }

    private String add(String a){
        return a+a;
    }
}
