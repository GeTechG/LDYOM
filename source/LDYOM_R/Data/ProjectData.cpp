#include "ProjectData.h"

#include "Localization/Localization.h"
#include "fmt/core.h"

std::vector<Actor>& ProjectData::getActors() {
	return actors_;
}

const char* names[200] = {"Douglas", "Aaron", "Brandon", "Dorothy", "Eric", "Carlos", "Shirley", "Carol", "Ralph", "Jeremy", "Lisa", "Kathleen", "Martin", "Teresa", "Donna", "Connie", "Sarah", "Cynthia", "Monica", "Michael", "Bruce", "Craig", "Mildred", "Timothy", "Evelyn", "Nicholas", "Angela", "Mary", "Andrea", "Dennis", "Samuel", "Stephanie", "Alice", "Amber", "Mark", "Rebecca", "Tina", "Joyce", "Laura", "Ernest", "Matthew", "Justin", "Marilyn", "Heather", "Donald", "Maria", "Christopher", "Ruby", "Susan", "Harold", "Victor", "Gerald", "Elizabeth", "Andrew", "Patricia", "Thomas", "Rachel", "Sherry", "Larry", "Brian", "Harry", "Eugene", "Lori", "Joe", "Ronald", "William", "Tiffany", "Randy", "Sylvia", "Frances", "Russell", "Cheryl", "April", "Kelly", "Brenda", "Alan", "Shawn", "Amanda", "Willie", "Karen", "Peter", "Sandra", "Stanley", "Debra", "Ryan", "Joseph", "Earl", "Danny", "Richard", "Norma", "Frank", "Amy", "Terry", "Antonio", "Ashley", "Ruth", "Helen", "Jose", "Kimberly", "Joan", "Ann", "Philip", "Martha", "Lois", "Judith", "Steve", "Tony", "Steven", "Nicole", "Bonnie", "Tracy", "Linda", "Paul", "Stephen", "Kenneth", "Charles", "John", "Jerry", "George", "Lauren", "Gregory", "Jesse", "Louis", "Louise", "Virginia", "Janet", "Gloria", "Jacqueline", "Henry", "Jimmy", "Carl", "Juan", "Johnny", "Jane", "Jennifer", "Walter", "Tammy", "Emma", "Denise", "Chris", "Catherine", "Melissa", "Patrick", "Todd", "Jonathan", "Irene", "Scott", "Grace", "Anthony", "Margaret", "Edward", "Keith", "Arthur", "Christine", "Albert", "Roy", "Wanda", "Bobby", "Doris", "Beverly", "Sharon", "Paula", "Pamela", "Jack", "Kevin", "Jason", "Lillian", "Howard", "Janice", "Benjamin", "Julia", "Daniel", "Robert", "David", "Betty", "Adam", "Lawrence", "Wendy", "Raymond", "Cindy", "Michelle", "Clarence", "Carolyn", "Judy", "Emily", "James", "Wayne", "Billy", "Nancy", "Barbara", "Jean", "Phyllis", "Rose", "Fred", "Jeffrey", "Kathy", "Joshua", "Jessica", "Roger", "Julie"};

void ProjectData::createNewActor() {
	const auto player = FindPlayerPed();
	const auto defaultName = names[rand() % 200];
	this->actors_.emplace_back(defaultName, player->GetPosition(), player->GetHeading());
	this->actors_.back().spawnEditorPed();
	this->actors_.back().updateLocation();
}
