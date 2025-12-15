BEGIN{
    FS="[][,]"
    num=0
}
{
    for(i=2;i<NF;i++){
        split($i,a,"-")
        switch(length(a)){
            case 1:
                j=a[1]
                host_names[num]=sprintf("%s%02d",$1,j)
                num++
                break
            case 2:
                for(j=a[1];j<=a[2];j++){
                    host_names[num]=sprintf("%s%02d",$1,j)
                    num++
                }
                break
            }
        delete a
    }
}
END{
    for(i=0;i<num;i++){
        for(j=0;j<mppn;j++){
            print host_names[i]
        }
    }
}
