Import-Module ActiveDirectory

$aResults = @()
$List = Get-Content ".\csv.csv"
            
ForEach($Item in $List){
    $Item = $Item.Trim()
    $a,$b = $Item.split(',',2)
    $a = $a.Trim()
    $b = $b.Trim()

    $User = Get-ADUser -Filter{givenName -like $a -and sn -like $b -and Enabled -eq $True} -Properties SamAccountName, GivenName, sn
#
    $count = 0
    if($User) {
	if($User.GetType().Name.Trim().Equals("Object[]")) {
	  $count = $User.length
        } else {
	  $count = 1
        }
#	echo $User.GetType() , $count
    }	
    $hItemDetails = New-Object -TypeName psobject -Property @{    
        CountOf = $count #.SamAccountName
	FirstName = $a  #$User.givenName
	LastName = $b #$User.sn
    }

    #Add data to array
    $aResults += $hItemDetails
}

$aResults | Export-CSV ".\Results.csv"

