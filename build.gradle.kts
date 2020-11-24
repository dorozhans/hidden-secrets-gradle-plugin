plugins {
    `java-gradle-plugin`
    `kotlin-dsl`
    id("io.gitlab.arturbosch.detekt") version "1.14.2"
}

group = "com.klaxit.hiddensecrets"
version = "0.1.0"

repositories {
    mavenCentral()
    google()
    jcenter()
}

dependencies {
    implementation("com.android.tools.build:gradle:4.0.0")

    testImplementation("io.kotlintest:kotlintest-runner-junit5:3.1.11")
    testImplementation("junit:junit:4.13.1")
}

configure<JavaPluginConvention> {
    sourceCompatibility = JavaVersion.VERSION_1_8
}

gradlePlugin {
    plugins {
        create("HiddenSecretsPlugin") {
            id = "com.klaxit.hiddensecrets"
            implementationClass = "com.klaxit.hiddensecrets.HiddenSecretsPlugin"
        }
    }
}

tasks.withType<Copy> {
    //Required by Gradle 7.0
    duplicatesStrategy = DuplicatesStrategy.INCLUDE
}

tasks.withType<Test> {
    useJUnitPlatform()
}