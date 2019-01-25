class InheritanceIncorrect{
    public static void main(String[] args){
        System.out.println(new A().work(24,new Z()));
    }
}
class X{
    int z;
    public int work(int y,B var){
        z=var.call(y);
        return var.get();
    }
}
class A extends X{
}
class Z{
    int q;
    public int get(){
        return q;
    }
    public int call(int m){
        q=m;
        return q;
    }
}
class B extends Z{
}