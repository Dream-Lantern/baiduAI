<?php  
//看是否是通过正常途径传递一个file过来了  
if(empty($_FILES["file"]))
{  
    //如果不是，则非正常打开此页  
    header("location: error.php");  
    exit;  
}  
else
{  
    header("Content-type: text/html; charset=utf-8");  
}  
//这是判断是否上传的file  
$canUpload=true;  
//这样就能够取得上传的文件名  
$fileName=$_FILES["file"]["name"];  
//通过对$fileName的处理，就能够取得上传的文件的后缀名  
$fileExtensions=strrchr($fileName, '.');  
//这样就能够取得上传文件的大小  
$fileSize=$_FILES["file"]["size"];  
//即将被打印的错误信息  
$errmsg="";  

//这里是文件的异常，一般不会出现  
if($_FILES["file"]["error"]>0)
{  
    $errmsg .= "异常：".$_FILES["file"]["error"]."！";  
    $canUpload=false;  
}  

//如果上传的文件名的后缀不是以下的几种则不能上传  
if($fileExtensions!=".bmp" && $fileExtensions!=".gif" && $fileExtensions!=".jpg" && $fileExtensions!=".jpeg" && $fileExtensions!=".png")
{  
    $errmsg .= "只能上传图片类型！后缀名必须为：.bmp,gif,jpg,jpeg,png任一！";  
    $canUpload=false;  
}  

//如果上传的文件大于10M则也不能上传  
if($fileSize>10*1024*1024)
{  
    $errmsg .= "文件太大！请少于10M！";  
    $canUpload=false;  
}  

//如果文件可以上传  
if($canUpload==true)
{  
    //保存在服务器的名字则是时间戳，加文件后缀名  
    //$saveName=time().$fileExtensions;  
    $saveName=$fileName;
    //取得服务器的目录的绝对路径。  
    $basepath=str_replace('\\','/',realpath(dirname(__FILE__).'/'))."/";  
    //然后则保存这个上传文件  
    //move_uploaded_file($_FILES["file"]["tmp_name"],$basepath."./upload/".$saveName);  
    move_uploaded_file($_FILES["file"]["tmp_name"],"/home/gujiguji/tmp/".$saveName);  
    echo $fileName."，上传成功！大小：".($fileSize/1024)."KB<br/><br/>";  

    // 测试 调用百度AI接口
    $command = './Botany '."/home/gujiguji/tmp/".$saveName;
    $result = passthru($command);
    print_r($result);

    echo "<br/><br/>";
    echo '<a href="javascript:history.back()" >返回上页</a>';
    echo "<br/>";
    echo '<a href="initial.html">返回首页</a>';
}  
else
{  
    echo $errmsg."<a href='initial.html'>返回</a>";  
}  

?> 
