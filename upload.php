<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<meta name="description" content="This web application was designed to analyse your circuit">
	<title>Circuit analysis tool</title>
   	<link rel="stylesheet" type="text/css" href="css/font-awesome.min.css">
   	<link rel="stylesheet" type="text/css" href="css/style.css">
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
	<style>
	</style>
</head>
<body align="center">
  <h1 style="color:rgb(120,120,120); " >Circuit Analysis Tool</h1>
	<form action="upload.php" method="post" enctype="multipart/form-data">
    	<label for="fileToUpload" class="label-input">
    		<i class="fa fa-upload"></i>
    		<span id="label_span">Select file to upload</span>
    	</label>
    <input type="file" name="fileToUpload" id="fileToUpload">
    <br><br>
  <button class="button"  type="submit" value="Upload File" name="submit" data-hover="Analyse"><span>Upload File</span></button>
</form>
	<ul align="left" class="fa-ul">
	<div class="tooltip">
		<pre style="padding: 3px; margin: 3px;font-style:oblique; font-size: 20px; font-family: sans-serif;">How to use it ?</pre><hr>
		<li><i class="fa fa-download" aria-hidden="true"></i> Download our <a title="Download" target = "blank" href ="https://l.facebook.com/l.php?u=https%3A%2F%2Fcdn.fbsbx.com%2Fv%2Ft59.2708-21%2F26914561_1534921406563410_4342818432882835456_n.txt%2Freadme.txt%3Foh%3Ddc96a51021feb8610f39d7995797bec1%26oe%3D5A65DA62%26dl%3D1&h=ATON0-PSyDbe0j3EpH2TCb3NaoGI0VUO7GXII62jzuNwlJov8uPk2IEUg-PQS0bnwHeumbYgMFGzTiTCWoOy4TEtXgU_hjgQZvYT_VWjqm0sVwU7YcGhGk-X_fdLNZR91deZwbmt36f1ML1bCIh9HrWIjw&s=1&enc=AZNe2krHI8bcL4nqMjH4QzkPtbLlk22lmVM-I0RsooQVmR5TMSsFnfJWf_6e4fmGcDT5GN0MvdLk2Cj1xD-mRacVZWOojw8_ne0Ya2aL3vlhKw">read me</a> file.</li>
</div>
	<li><i class="fa fa-pencil-square-o" aria-hidden="true"></i>
	 Wrtie your input file.</li>
	<li><i class="fa fa-upload" aria-hidden="true"></i>
    Upload your input file.</li>
  	<li><i class="fa fa-spinner" aria-hidden="true" ></i> The application 	processes your input file.</li>
	<li><i class="fa fa-check-circle-o" aria-hidden="true"></i> Your input circuit will be analysed.</li>
	<hr width="320" align="left">
	</ul>
	<script src="file.js"></script>
	<?php
$target_dir = "uploads/";
$name = rand(0, 10000000) . ".txt";
$target_file = $target_dir . $name;
$uploadOk = 1;
// Check file size
if ($_FILES["fileToUpload"]["size"] > 500000) {
    echo "Sorry, your file is too large.";
    $uploadOk = 0;
}
// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
    echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else {
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
        $output = shell_exec('./analyze uploads/'.$name);
        echo "<pre class='border'>$output</pre>";
    } else {
        echo "Sorry, there was an error uploading your file.";
    }
}
?>
</body>
</html>