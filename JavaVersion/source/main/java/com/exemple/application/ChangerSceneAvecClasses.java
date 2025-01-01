package com.exemple.application;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public class ChangerSceneAvecClasses extends Application {

    private Stage primaryStage; // Main Stage

    @Override
    public void start(Stage primaryStage) {
        this.primaryStage = primaryStage;

        // Initialisation de la Scène 1
        Scene1 scene1 = new Scene1(primaryStage);
        primaryStage.setTitle("Welcome to the diary");
        primaryStage.setScene(scene1.getScene());
        primaryStage.show();
    }

    // Scene 1
    private class Scene1 {
        private final Scene scene;

        public Scene1(Stage primaryStage) {
            Label texteCentre = new Label("Bienvenue sur la Scène 1");
            Button bouton1 = new Button("Log In");
            Button bouton2 = new Button("Sign In");

            bouton1.setOnAction(event -> primaryStage.setScene(new Scene2(primaryStage).getScene()));
            bouton2.setOnAction(event -> primaryStage.setScene(new Scene3(primaryStage).getScene()));

            VBox boutons = new VBox(10, bouton1, bouton2);
            boutons.setStyle("-fx-alignment: center; -fx-spacing: 20px;");
            BorderPane layout = new BorderPane();
            layout.setCenter(texteCentre);
            layout.setBottom(boutons);

            this.scene = new Scene(layout, 400, 200);
        }

        public Scene getScene() {
            return scene;
        }
    }

    // Scene 2 : LOG IN
    private class Scene2 {
        private final Scene scene;

        public Scene2(Stage primaryStage) {
            Label texteScene2 = new Label("Vous êtes sur la Scène 2");
            Button retour = new Button("Retour à la Scène 1");

            retour.setOnAction(event -> primaryStage.setScene(new Scene1(primaryStage).getScene()));

            StackPane layout = new StackPane(texteScene2, retour);
            StackPane.setAlignment(retour, javafx.geometry.Pos.BOTTOM_CENTER);
            this.scene = new Scene(layout, 400, 200);
        }

        public Scene getScene() {
            return scene;
        }
    }

    // Scene 3 : SIGN IN
    private class Scene3 {
        private final Scene scene;

        public Scene3(Stage primaryStage) {
            Label texteScene3 = new Label("Vous êtes sur la Scène 3");
            Button retour = new Button("Retour à la Scène 1");

            retour.setOnAction(event -> primaryStage.setScene(new Scene1(primaryStage).getScene()));

            StackPane layout = new StackPane(texteScene3, retour);
            StackPane.setAlignment(retour, javafx.geometry.Pos.BOTTOM_CENTER);
            this.scene = new Scene(layout, 400, 200);
        }

        public Scene getScene() {
            return scene;
        }
    }

    public static void main(String[] args) {
        launch(args);
    }
}
